// Copyright (C) 2002-2010 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_MESH_CACHE_H_INCLUDED__
#define __C_MESH_CACHE_H_INCLUDED__

#include "IMeshCache.h"
#include "rbtree_spec.h"

namespace irr
{

namespace scene
{
	class CMeshCache : public IMeshCache
	{
	public:

		virtual ~CMeshCache();

		//! Adds a mesh to the internal list of loaded meshes.
		/** Usually, ISceneManager::getMesh() is called to load a mesh from file.
		That method searches the list of loaded meshes if a mesh has already been loaded and
		returns a pointer to if	it is in that list and already in memory. Otherwise it loads
		the mesh. With IMeshCache::addMesh(), it is possible to pretend that a mesh already
		has been loaded. This method can be used for example by mesh loaders who need to
		load more than one mesh with one call. They can add additional meshes with this
		method to the scene manager. The COLLADA loader for example uses this method.
		\param filename: Filename of the mesh. When called ISceneManager::getMesh() with this
		parameter, the method will return the mesh parameter given with this method.
		\param mesh: Pointer to a mesh which will now be referenced by this name. */
		virtual void addMesh(const io::path& filename, IAnimatedMesh* mesh);

		//! Removes a mesh from the cache.
		/** After loading a mesh with getMesh(), the mesh can be removed from the cache
		using this method, freeing a lot of memory. */
		virtual void removeMesh(IAnimatedMesh* mesh);

		//! Removes a mesh from the cache.
		/** After loading a mesh with getMesh(), the mesh can be removed from the cache
		using this method, freeing a lot of memory. */
		virtual void removeMesh(const IMesh* const mesh);

		//! Returns amount of loaded meshes in the cache.
		/** You can load new meshes into the cache using getMesh() and addMesh().
		If you ever need to access the internal mesh cache, you can do this using
		removeMesh(), getMeshNumber(), and getMeshFilename() */
		virtual u32 getMeshCount() const;

		//! Returns a mesh based on its name.
		/** \param name Name of the mesh. Usually a filename.
		\return Pointer to the mesh or 0 if there is none with this number. */
		virtual IAnimatedMesh* getMeshByName(const io::path& name);

		//! Get the name of a loaded mesh, if there is any.
		/** \param mesh Pointer to mesh to query.
		\return The name if mesh was found and has a name, else	the path is empty. */
		virtual const io::SNamedPath& getMeshName(const IAnimatedMesh* const mesh) const;

		//! Get the name of a loaded mesh, if there is any.
		/** \param mesh Pointer to mesh to query.
		\return The name if mesh was found and has a name, else	the path is empty. */
		virtual const io::SNamedPath& getMeshName(const IMesh* const mesh) const;

		//! Renames a loaded mesh.
		/**\param mesh Mesh to be renamed.
		\param name New name for the mesh.
		\return True if mesh was renamed. */
		virtual bool renameMesh(IAnimatedMesh* mesh, const io::path& name);

		//! Renames a loaded mesh.
		/**\param mesh Mesh to be renamed.
		\param name New name for the mesh.
		\return True if mesh was renamed. */
		virtual bool renameMesh(IMesh* mesh, const io::path& name);

		//! returns if a mesh already was loaded
		virtual bool isMeshLoaded(const io::path& name);

		//! Clears the whole mesh cache, removing all meshes.
		virtual void clear();

		//! Clears all meshes that are held in the mesh cache but not used anywhere else.
		virtual void clearUnusedMeshes();

	protected:

		//! loaded meshes
		core::droptree<io::SNamedPath, IAnimatedMesh *> Meshes;
		core::deltree<const IAnimatedMesh *, const io::SNamedPath *> RevMeshes;
		core::map<const IMesh *, const io::SNamedPath *> IMeshes;
	};


} // end namespace scene
} // end namespace irr


#endif

