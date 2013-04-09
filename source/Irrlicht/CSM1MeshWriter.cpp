// Copyright (C) 2013 Lauri Kasanen
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "IrrCompileConfig.h"

#ifdef _IRR_COMPILE_WITH_SM1_WRITER_

#include "CSM1MeshWriter.h"
#include "os.h"
#include "IMesh.h"
#include "IMeshBuffer.h"
#include "IWriteFile.h"
#include "ITexture.h"

#ifndef _IRR_USE_NON_SYSTEM_ZLIB_
#include <zlib.h> // use system lib
#else
#include "zlib/zlib.h"
#endif

namespace irr
{
namespace scene
{

using namespace core;
using namespace video;

static int writesm(io::IWriteFile* const f, const char texture[],
			const sm1_vert * const v, const u16 verts,
			const u16 * const i, const u32 indices) {

	if (!v || !i)
		return 1;

	const u32 texlen = strlen(texture);
	const u32 buflen = verts * sizeof(sm1_vert) + 7 + texlen + indices * sizeof(u16);
	const u32 bound = compressBound(buflen);

	// Header: magic bytes, version, uncompressed size
	f->write("sm", 2);
	u8 utmp = SM_VER;
	if (f->write(&utmp, 1) != 1 || f->write(&buflen, sizeof(u32)) != sizeof(u32)) {
		return 1;
	}

	// Create a temp buffer to be compressed
	char * const buf = (char *) calloc(buflen, 1);
	char *ptr = buf;

	// Fill
	memcpy(ptr, &verts, sizeof(u16));
	ptr += sizeof(u16);

	memcpy(ptr, &indices, sizeof(u32));
	ptr += sizeof(u32);

	*ptr = texlen;
	ptr++;

	memcpy(ptr, texture, texlen);
	ptr += texlen;

	memcpy(ptr, v, verts * sizeof(sm1_vert));
	ptr += verts * sizeof(sm1_vert);

	memcpy(ptr, i, indices * sizeof(u16));

	// Compress
	char *compressed = (char *) calloc(bound, 1);
	uLongf destlen = bound;

	int ret = compress2((Bytef *) compressed, &destlen, (Bytef *) buf, buflen, 9);
	free(buf);
	if (ret != Z_OK) {
		free(compressed);

		return 1;
	}

	// Write payload
	u32 tmp = destlen;
	if (f->write(compressed, tmp) != (s32) tmp) {
		free(compressed);

		return 1;
	}

	free(compressed);

	return 0;
}

CSM1MeshWriter::CSM1MeshWriter(io::IFileSystem *fs): FileSystem(fs)
{
	#ifdef _DEBUG
	setDebugName("CSM1MeshWriter");
	#endif
}


//! Returns the type of the mesh writer
EMESH_WRITER_TYPE CSM1MeshWriter::getType() const
{
	return EMWT_SM1;
}

//! writes a mesh
bool CSM1MeshWriter::writeMesh(io::IWriteFile* file, IMesh* const mesh, s32 flags)
{
	if (!file || !mesh)
		return false;
#ifdef __BIG_ENDIAN__
	os::Printer::log("SM1 does not support big-endian systems.", ELL_ERROR);
	return false;
#endif

	u32 ret = 0, i;
	const u32 max = mesh->getMeshBufferCount();
	const stringc origname = file->getFileName();

	for (i = 0; i < max; i++) {

		if (i) {
			stringc name = origname + "_";
			name += i;
			file = FileSystem->createAndWriteFile(name);
			if (!file) return false;
		}

		const IMeshBuffer * const mb = mesh->getMeshBuffer(i);

		if (mb->getIndexType() != EIT_16BIT) {
			os::Printer::log("SM1 only supports 16bit indices.", ELL_ERROR);
			return false;
		}
		if (mb->getVertexType() != EVT_STANDARD) {
			os::Printer::log("SM1 only supports standard vertices.", ELL_ERROR);
			return false;
		}

		// Does it have a texture?
		const char *texname = "";

		const ITexture * const tex = mb->getMaterial().getTexture(0);
		if (tex) {
			texname = tex->getName().getPath().c_str();
		}

		// Convert it to SM1 vertex format
		const u32 icount = mb->getIndexCount();
		const u32 vcount = mb->getVertexCount();

		if (vcount > USHRT_MAX) {
			os::Printer::log("Too many vertices for SM1.", ELL_ERROR);
			return false;
		}

		sm1_vert * const vert = (sm1_vert *) calloc(vcount, sizeof(sm1_vert));
		const S3DVertex * const inv = (S3DVertex *) mb->getVertices();

		u32 j;
		for (j = 0; j < vcount; j++) {
			vert[j].x = inv[j].Pos.X;
			vert[j].y = inv[j].Pos.Y;
			vert[j].z = inv[j].Pos.Z;

			vert[j].nx = inv[j].Normal.X;
			vert[j].ny = inv[j].Normal.Y;
			vert[j].nz = inv[j].Normal.Z;

			vert[j].uvx = inv[j].TCoords.X;
			vert[j].uvy = inv[j].TCoords.Y;

			vert[j].r = inv[j].Color.getRed();
			vert[j].g = inv[j].Color.getGreen();
			vert[j].b = inv[j].Color.getBlue();
			vert[j].a = inv[j].Color.getAlpha();
		}

		// Write
		ret |= writesm(file, texname, vert, vcount, mb->getIndices(), icount);

		free(vert);
	}

	return ret == 0;
}

} // end namespace
} // end namespace

#endif // _IRR_COMPILE_WITH_SM1_WRITER_

