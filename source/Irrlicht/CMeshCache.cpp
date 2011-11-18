// Copyright (C) 2002-2010 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CMeshCache.h"
#include "IAnimatedMesh.h"
#include "IMesh.h"

namespace irr
{
namespace scene
{

static const io::SNamedPath emptyNamedPath;


CMeshCache::~CMeshCache()
{
	clear();
}


//! adds a mesh to the list
void CMeshCache::addMesh(const io::path& filename, IAnimatedMesh* mesh)
{
	mesh->grab();

	const io::SNamedPath *copy = new io::SNamedPath(filename);

	Meshes.insert(filename, mesh);

	if (!RevMeshes.find(mesh)) {
		RevMeshes.insert(mesh, copy);
		IMeshes.insert(mesh->getMesh(0), copy);
	}
	else delete copy;
}


//! Removes a mesh from the cache.
void CMeshCache::removeMesh(IAnimatedMesh* mesh)
{
	if ( !mesh )
		return;

	revnode *r = RevMeshes.find(mesh);

	if (r) {

		const io::SNamedPath *s = r->getValue();

		IMeshes.remove(mesh->getMesh(0));
		Meshes.remove(*s);
		RevMeshes.remove(mesh);
	}

}


//! Removes a mesh from the cache.
void CMeshCache::removeMesh(const IMesh* const mesh)
{
	if ( !mesh )
		return;

	inode *i = IMeshes.find(mesh);

	if (i) {
		const io::SNamedPath *s = i->getValue();

		IMeshes.remove(mesh);
		Meshes.remove(*s);
		RevMeshes.remove(Meshes.find(*s)->getValue());
	}
}


//! Returns amount of loaded meshes
u32 CMeshCache::getMeshCount() const
{
	return Meshes.size();
}


//! Returns a mesh based on its name.
IAnimatedMesh* CMeshCache::getMeshByName(const io::path& name)
{
	meshnode *m = Meshes.find(name);

	if (m) return m->getValue();

	return 0;
}

//! Get the name of a loaded mesh, if there is any.
const io::SNamedPath& CMeshCache::getMeshName(const IAnimatedMesh* const mesh) const
{
	if(!mesh)
		return emptyNamedPath;

	revnode *r = RevMeshes.find(mesh);

	if (r)
		return *r->getValue();

	return emptyNamedPath;
}

//! Get the name of a loaded mesh, if there is any.
const io::SNamedPath& CMeshCache::getMeshName(const IMesh* const mesh) const
{
	if (!mesh)
		return emptyNamedPath;

	// IMesh may actually be an IAnimatedMesh, so do a direct comparison
	// as well as getting an IMesh from our stored IAnimatedMeshes
	revnode *r = RevMeshes.find((IAnimatedMesh *) mesh);

	if (r) {
		return *r->getValue();
	}

	inode *i = IMeshes.find(mesh);

	if (i) {
		return *i->getValue();
	}

	return emptyNamedPath;
}

//! Renames a loaded mesh.
bool CMeshCache::renameMesh(IAnimatedMesh* mesh, const io::path& name)
{
	revnode *r = RevMeshes.find(mesh);
	if (!r) return false;

	const io::SNamedPath *n = r->getValue();

	const io::SNamedPath *copy = new io::SNamedPath(name);

	// Avoid accidental deletion
	mesh->grab();

	IMeshes.remove(mesh->getMesh(0));
	Meshes.remove(*n);
	RevMeshes.remove(mesh);

	Meshes.insert(name, mesh);
	RevMeshes.insert(mesh, copy);
	IMeshes.insert(mesh->getMesh(0), copy);

	return true;
}

//! Renames a loaded mesh.
bool CMeshCache::renameMesh(IMesh* mesh, const io::path& name)
{
	inode *i = IMeshes.find(mesh);
	if (!i) return false;

	const io::SNamedPath *n = i->getValue();

	const io::SNamedPath *copy = new io::SNamedPath(name);

	IAnimatedMesh *ani = Meshes.find(*n)->getValue();

	// Avoid accidental deletion
	mesh->grab();

	IMeshes.remove(mesh);
	Meshes.remove(*n);
	RevMeshes.remove(ani);

	Meshes.insert(name, ani);
	RevMeshes.insert(ani, copy);
	IMeshes.insert(mesh, copy);

	return true;
}

//! returns if a mesh already was loaded
bool CMeshCache::isMeshLoaded(const io::path& name)
{
	return getMeshByName(name) != NULL;
}


//! Clears the whole mesh cache, removing all meshes.
void CMeshCache::clear()
{
	Meshes.clear();
	RevMeshes.clear();
	IMeshes.clear();
}

//! Clears all meshes that are held in the mesh cache but not used anywhere else.
void CMeshCache::clearUnusedMeshes()
{

	core::array<io::SNamedPath> *a = Meshes.findUnused();
	core::array<io::SNamedPath> &b = *a;

	for (u32 i = 0; i < a->size(); i++) {

		IAnimatedMesh *m = Meshes.find(b[i])->getValue();

		IMeshes.remove(m->getMesh(0));
		RevMeshes.remove(m);
		Meshes.remove(b[i]);
	}

	delete a;
}


} // end namespace scene
} // end namespace irr

