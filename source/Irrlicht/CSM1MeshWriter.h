// Copyright (C) 2013 Lauri Kasanen
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __IRR_SM1_MESH_WRITER_H_INCLUDED__
#define __IRR_SM1_MESH_WRITER_H_INCLUDED__

#include "CSM1Mesh.h"
#include "IMeshWriter.h"
#include "IFileSystem.h"

namespace irr
{
namespace scene
{

//! class to write SM1 mesh files
class CSM1MeshWriter : public IMeshWriter
{
public:

	CSM1MeshWriter(io::IFileSystem *fs);

	//! Returns the type of the mesh writer
	virtual EMESH_WRITER_TYPE getType() const;

	//! writes a mesh
	virtual bool writeMesh(io::IWriteFile* file, scene::IMesh* mesh, s32 flags=EMWF_NONE);

private:
	io::IFileSystem *FileSystem;
};

} // end namespace
} // end namespace

#endif
