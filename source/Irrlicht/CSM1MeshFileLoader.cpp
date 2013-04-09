// Copyright (C) 2013 Lauri Kasanen
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "IrrCompileConfig.h"
#ifdef _IRR_COMPILE_WITH_SM1_LOADER_

#include "CSM1MeshFileLoader.h"
#include "IReadFile.h"
#include "os.h"
#include "SMesh.h"
#include "SAnimatedMesh.h"
#include "SMeshBuffer.h"

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
using namespace io;

static int readsm(IReadFile * const f, char **texture, sm1_vert **v, u16 *verts,
		u16 **i, u32 *indices) {

	// How long is it?
	const int len = f->getSize();
	if (len < 1)
		return 1;

	// Read to RAM.
	char *buf = (char *) calloc(len, 1);

	if (f->read(buf, len) != len) {
		free(buf);
		return 1;
	}

	// Is it a SM file, of the correct version?
	if (buf[0] != 's' || buf[1] != 'm' || buf[2] != SM_VER) {
		free(buf);
		return 1;
	}

	u32 payloadlen = *(u32 *) &buf[3];

	// Allocate payload buffer
	char * const payload = (char *) calloc(payloadlen, 1);

	// Uncompress it
	uLongf tmplen = payloadlen;
	int ret = uncompress((Bytef *) payload, &tmplen, (Bytef *) buf + 7, len - 7);

	if (ret != Z_OK) {
		free(buf);
		free(payload);
		return 1;
	}

	free(buf);

	// Read variables
	const char *ptr = payload;

	*verts = *(u16 *) ptr;
	ptr += sizeof(u16);

	*indices = *(u32 *) ptr;
	ptr += sizeof(u32);

	u8 texlen = *ptr;
	ptr++;

	char *textmp = (char *) calloc(texlen + 1, 1);
	memcpy(textmp, ptr, texlen);
	ptr += texlen;
	*texture = textmp;

	// Only verts and indices remaining
	sm1_vert *vtmp = (sm1_vert *) calloc(*verts, sizeof(sm1_vert));
	u16 *itmp = (u16 *) calloc(*indices, sizeof(u16));

	memcpy(vtmp, ptr, *verts * sizeof(sm1_vert));
	ptr += *verts * sizeof(sm1_vert);

	memcpy(itmp, ptr, *indices * sizeof(u16));

	*v = vtmp;
	*i = itmp;

	free(payload);
	return 0;
}

// constructor
CSM1MeshFileLoader::CSM1MeshFileLoader(IVideoDriver *drv): Driver(drv)
{
}


CSM1MeshFileLoader::~CSM1MeshFileLoader()
{
}


//! returns true if the file maybe is able to be loaded by this class
bool CSM1MeshFileLoader::isALoadableFileExtension(const io::path& filename) const
{
	return core::hasFileExtension(filename, "sm1");
}


//! creates/loads an animated mesh from the file.
IAnimatedMesh* CSM1MeshFileLoader::createMesh(io::IReadFile* file)
{
	if (!file)
		return 0;
#ifdef __BIG_ENDIAN__
	os::Printer::log("SM1 does not support big-endian systems.", ELL_ERROR);
	return 0;
#endif

	// Go
	sm1_vert *v;
	u16 vcount;
	u16 *ind;
	u32 icount;
	char *tex;

	int ret = readsm(file, &tex, &v, &vcount, &ind, &icount);
	if (ret) {
		os::Printer::log("Failed to load SM1 mesh.", ELL_ERROR);
		return 0;
	}

	// Convert
	SMesh * const m = new SMesh();
	SMeshBuffer * const mb = new SMeshBuffer();

	mb->Vertices.reallocate(vcount);
	mb->Indices.set_used(icount);
	u16 * const outi = mb->getIndices();

	if (strlen(tex) > 2) {
		ITexture * const itex = Driver->getTexture(tex);
		if (itex)
			mb->getMaterial().setTexture(0, itex);
	}

	u32 i;
	for (i = 0; i < vcount; i++) {
		mb->Vertices.push_back(S3DVertex(v[i].x, v[i].y, v[i].z,
					v[i].nx, v[i].ny, v[i].nz,
					SColor(v[i].a, v[i].r, v[i].g, v[i].b),
					v[i].uvx, v[i].uvy));
	}

	memcpy(outi, ind, icount * sizeof(u16));

	m->addMeshBuffer(mb);
	mb->drop();
	mb->recalculateBoundingBox();
	m->recalculateBoundingBox();

	// Cleanup
	free(v);
	free(ind);
	free(tex);

	// if we managed to create a mesh, return it
	SAnimatedMesh * const am = new SAnimatedMesh();
	am->Type = EAMT_OBJ;
	am->addMesh(m);
	am->recalculateBoundingBox();

	return am;
}

} // end namespace scene
} // end namespace irr

#endif // _IRR_COMPILE_WITH_SM1_LOADER_

