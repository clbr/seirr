// Copyright (C) 2014 Lauri Kasanen
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __IRR_B3D_MESH_WRITER_H_INCLUDED__
#define __IRR_B3D_MESH_WRITER_H_INCLUDED__

#include "IMeshWriter.h"
#include "IFileSystem.h"
#include "SB3DStructs.h"

namespace irr
{
namespace scene
{

//! class to write B3D mesh files
class CB3DMeshWriter : public IMeshWriter
{
public:

	CB3DMeshWriter(io::IFileSystem *fs);

	//! Returns the type of the mesh writer
	virtual EMESH_WRITER_TYPE getType() const _IRR_OVERRIDE_;

	//! writes a mesh
	virtual bool writeMesh(io::IWriteFile* file, scene::IMesh* mesh, s32 flags=EMWF_NONE) _IRR_OVERRIDE_;

private:
	io::IFileSystem *FileSystem;
	u32 size;

	void write(io::IWriteFile* file, const void *ptr, const u32 bytes);
};

} // end namespace
} // end namespace

#endif
