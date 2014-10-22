// Copyright (C) 2014 Lauri Kasanen
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_IQE_MESH_FILE_LOADER_H_INCLUDED__
#define __C_IQE_MESH_FILE_LOADER_H_INCLUDED__

#include "IMeshLoader.h"
#include "ISceneManager.h"
#include "irrString.h"
#include "SMeshBuffer.h"
#include "irrMap.h"

namespace irr
{
namespace scene
{

class SMesh;

//! Meshloader capable of loading iqe meshes.
class CIQEMeshFileLoader : public IMeshLoader
{
public:

	//! Constructor
	CIQEMeshFileLoader(video::IVideoDriver *idrv, io::IFileSystem *ifs);

	//! destructor
	virtual ~CIQEMeshFileLoader() _IRR_OVERRIDE_;

	//! returns true if the file maybe is able to be loaded by this class
	//! based on the file extension (e.g. ".iqe")
	virtual bool isALoadableFileExtension(const io::path& filename) const _IRR_OVERRIDE_;

	//! creates/loads an animated mesh from the file.
	//! \return Pointer to the created mesh. Returns 0 if loading failed.
	//! If you no longer need the mesh, you should call IAnimatedMesh::drop().
	//! See IReferenceCounted::drop() for more information.
	virtual IAnimatedMesh* createMesh(io::IReadFile* file) _IRR_OVERRIDE_;

private:
	void parse(SMesh*, const char*, const u16, const io::path&) const;

	video::IVideoDriver *drv;
	io::IFileSystem *fs;
};

} // end namespace scene
} // end namespace irr

#endif

