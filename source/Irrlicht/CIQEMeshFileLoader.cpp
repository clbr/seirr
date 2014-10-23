// Copyright (C) 2014 Lauri Kasanen
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "IrrCompileConfig.h"
#ifdef _IRR_COMPILE_WITH_IQE_LOADER_

#include "CIQEMeshFileLoader.h"
#include "IMeshManipulator.h"
#include "IVideoDriver.h"
#include "SMesh.h"
#include "SMeshBuffer.h"
#include "SAnimatedMesh.h"
#include "IReadFile.h"
#include "IAttributes.h"
#include "fast_atof.h"
#include "coreutil.h"
#include "os.h"
#include "IFileSystem.h"

namespace irr
{
namespace scene
{

using namespace video;
using namespace core;

//! Constructor
CIQEMeshFileLoader::CIQEMeshFileLoader(IVideoDriver *idrv, io::IFileSystem *ifs)
: drv(idrv), fs(ifs)
{
	#ifdef _DEBUG
	setDebugName("CIQEMeshFileLoader");
	#endif
}


//! destructor
CIQEMeshFileLoader::~CIQEMeshFileLoader()
{
}


//! returns true if the file maybe is able to be loaded by this class
//! based on the file extension (e.g. ".bsp")
bool CIQEMeshFileLoader::isALoadableFileExtension(const io::path& filename) const
{
	return core::hasFileExtension ( filename, "iqe" );
}

static u32 countVerts(const char *buf) {
	u32 num = 0;
	const char *ptr = buf;
	while ((ptr = strstr(ptr, "\nvp "))) {
		num++;
		ptr++;
	}

	return num;
}

static const char *getLine(const char * const ptr, char buf[], const u32 bufsize) {
	const char *end = strchr(ptr, '\n');
	if (!end) end = ptr + strlen(ptr);

	const u32 len = end - ptr;
	if (len >= bufsize) {
		os::Printer::log("Warning: File contains a too long line", ELL_WARNING);
		buf[0] = '\0';
		return end;
	}

	memcpy(buf, ptr, len);
	buf[len] = '\0';

	return end;
}

static ITexture *xgetTexture(const io::path &name, IVideoDriver *drv, io::IFileSystem *fs) {
	// Get a texture without spewing warnings
	ITexture *out = NULL;

	out = drv->findTexture(name);
	if (out) return out;

	if (fs->existFile(name))
		return drv->getTexture(name);

	return NULL;
}

void CIQEMeshFileLoader::parse(SMesh * const mesh, const char *ptr,
				const u16 maxVerts, const io::path &relPath) const {
	enum {
		bufsize = 1024
	};

	char buf[bufsize];

	SMeshBuffer *mb = NULL;
	u16 N, P, T, C; // the attributes may be interleaved, or not, so individual counters
	N = P = T = C = 0;

	#define cmd(x) if (!strncmp(x, buf, sizeof(x) - 1))

	float f1, f2, f3, f4;
	s32 i1, i2, i3;

	const S3DVertex base(0, 0, 0, 0, 1, 0, SColor(255, 255, 255, 255), 0, 0);

	while (1) {
		ptr = getLine(ptr, buf, bufsize);
		if (*ptr) ptr++;
		if (buf[0] == '#')
			continue;

		cmd("vp ") {
			if (!mb) continue;
			while (mb->getVertexCount() <= P)
				mb->Vertices.push_back(base);

			f1 = f2 = f3 = 0;
			sscanf(buf + 3, "%f %f %f", &f1, &f2, &f3);

			mb->Vertices[P].Pos.X = f1;
			mb->Vertices[P].Pos.Y = f2;
			mb->Vertices[P].Pos.Z = f3;

			P++;
		} else cmd("vn ") {
			if (!mb) continue;
			while (mb->getVertexCount() <= N)
				mb->Vertices.push_back(base);

			f1 = f2 = f3 = 0;
			sscanf(buf + 3, "%f %f %f", &f1, &f2, &f3);

			mb->Vertices[N].Normal.X = f1;
			mb->Vertices[N].Normal.Y = f2;
			mb->Vertices[N].Normal.Z = f3;

			N++;
		} else cmd("vt ") {
			if (!mb) continue;
			while (mb->getVertexCount() <= T)
				mb->Vertices.push_back(base);

			f1 = f2 = 0;
			sscanf(buf + 3, "%f %f", &f1, &f2);

			mb->Vertices[T].TCoords.X = f1;
			mb->Vertices[T].TCoords.Y = f2;

			T++;
		} else cmd("vc ") {
			if (!mb) continue;
			while (mb->getVertexCount() <= C)
				mb->Vertices.push_back(base);

			f1 = f2 = f3 = 0;
			f4 = 1;
			sscanf(buf + 3, "%f %f %f %f", &f1, &f2, &f3, &f4);

			mb->Vertices[C].Color.setRed(f1 * 255);
			mb->Vertices[C].Color.setGreen(f2 * 255);
			mb->Vertices[C].Color.setBlue(f3 * 255);
			mb->Vertices[C].Color.setAlpha(f4 * 255);

			C++;
		} else cmd("mesh ") {
			if (mb) {
				mb->recalculateBoundingBox();
				mesh->addMeshBuffer(mb);
				mb->drop();
			}
			mb = new SMeshBuffer();
			N = P = T = C = 0;

			mb->Vertices.reallocate(maxVerts);
			mb->Indices.reallocate(maxVerts * 3);
		} else cmd("comment ") {
			break;
		} else cmd("material ") {
			if (!mb) continue;

			const char *start = buf + 9;

			if (strstr(start, "doublesided;"))
				mb->getMaterial().setFlag(EMF_BACK_FACE_CULLING, false);

			while (strchr(start, ';')) {
				start = strchr(start, ';') + 1;
			}

			// Whatever remains is the texture, maybe sans extension

			static const char * const extension[] =
			{
				".jpg",
				".jpeg",
				".png",
				".dds",
				".tga",
				".bmp",
				".pcx"
			};
			const u32 exts = sizeof(extension) / sizeof(extension[0]);

			ITexture *tex = xgetTexture(start, drv, fs);
			for (u32 i = 0; !tex && i < exts; i++) {
				stringc tmp = start;
				tmp += extension[i];

				tex = xgetTexture(tmp, drv, fs);
			}

			if (!tex) {
				tex = xgetTexture(relPath + start, drv, fs);
				for (u32 i = 0; !tex && i < exts; i++) {
					stringc tmp = relPath;
					tmp += start;
					tmp += extension[i];

					tex = xgetTexture(tmp, drv, fs);
				}
			}

			mb->getMaterial().setTexture(0, tex);
		} else cmd("fm ") {
			if (!mb) continue;

			i1 = i2 = i3 = 0;
			sscanf(buf + 3, "%d %d %d", &i1, &i2, &i3);

			if (i1 < 0)
				i1 = mb->Vertices.size() + i1;
			if (i2 < 0)
				i2 = mb->Vertices.size() + i2;
			if (i3 < 0)
				i3 = mb->Vertices.size() + i3;

			// Note the reversed winding order
			mb->Indices.push_back(i3);
			mb->Indices.push_back(i2);
			mb->Indices.push_back(i1);
		}

		if (!*ptr) break;
	}

	#undef cmd

	if (!mb)
		return;

	mb->recalculateBoundingBox();
	mesh->addMeshBuffer(mb);
	mb->drop();
}

//! creates/loads an animated mesh from the file.
//! \return Pointer to the created mesh. Returns 0 if loading failed.
//! If you no longer need the mesh, you should call IAnimatedMesh::drop().
//! See IReferenceCounted::drop() for more information.
IAnimatedMesh* CIQEMeshFileLoader::createMesh(io::IReadFile* file)
{
	const long filesize = file->getSize();
	if (!filesize)
		return 0;

	c8* buf = new c8[filesize + 1];
	memset(buf, 0, filesize + 1);
	file->read((void*)buf, filesize);

	const u32 totalVerts = countVerts(buf);
	if (totalVerts > USHRT_MAX) {
		os::Printer::log("Error: File contains too many vertices, over 65k (16-bit)",
					ELL_ERROR);
		delete [] buf;
		return 0;
	}

	if (strstr(buf, "\nfa ")) {
		os::Printer::log("Error: No support for IQE with global vertices",
					ELL_ERROR);
		delete [] buf;
		return 0;
	}

	if (strstr(buf, "\nvertexarray ") ||
		strstr(buf, "\nvb ") ||
		strstr(buf, "\nsmooth") ||
		strstr(buf, "\njoint ") ||
		strstr(buf, "\nanimation ") ||
		strstr(buf, "\npq ")) {
		os::Printer::log("Warning: IQE file contains unsupported features",
					ELL_WARNING);
	}

	const io::path relPath = fs->getFileDir(file->getFileName()) + "/";

	SMesh* mesh = new SMesh();

	parse(mesh, buf, totalVerts, relPath);

	// Create the Animated mesh if there's anything in the mesh
	SAnimatedMesh* animMesh = 0;
	if ( 0 != mesh->getMeshBufferCount() )
	{
		mesh->recalculateBoundingBox();
		animMesh = new SAnimatedMesh();
		animMesh->Type = EAMT_OBJ;
		animMesh->addMesh(mesh);
		animMesh->recalculateBoundingBox();
	}

	// Clean up the allocated file contents
	delete [] buf;
	mesh->drop();

	return animMesh;
}


} // end namespace scene
} // end namespace irr

#endif // _IRR_COMPILE_WITH_IQE_LOADER_

