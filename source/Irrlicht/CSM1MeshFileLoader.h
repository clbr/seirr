// Copyright (C) 2013 Lauri Kasanen
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_SM1_MESH_FILE_LOADER_H_INCLUDED__
#define __C_SM1_MESH_FILE_LOADER_H_INCLUDED__

#include "IMeshLoader.h"
#include "CDynamicMeshBuffer.h"
#include "CSM1Mesh.h"
#include "IVideoDriver.h"

namespace irr
{
namespace scene
{

//! Meshloader capable of loading sm1 meshes.
class CSM1MeshFileLoader : public IMeshLoader
{
public:

	//! Constructor
	CSM1MeshFileLoader(video::IVideoDriver *drv);

	//! Destructor
	virtual ~CSM1MeshFileLoader();

	//! returns true if the file maybe is able to be loaded by this class
	//! based on the file extension (e.g. ".sm1")
	virtual bool isALoadableFileExtension(const io::path& filename) const;

	//! creates/loads an animated mesh from the file.
	virtual IAnimatedMesh* createMesh(io::IReadFile* file);

private:

	video::IVideoDriver *Driver;
};

} // end namespace scene
} // end namespace irr

#endif

