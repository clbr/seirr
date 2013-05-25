// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_MESH_CACHE_H_INCLUDED__
#define __I_MESH_CACHE_H_INCLUDED__

#include "IReferenceCounted.h"
#include "path.h"

namespace irr
{

namespace scene
{
	class IMesh;
	class IAnimatedMesh;
	class IAnimatedMeshSceneNode;
	class IMeshLoader;

	//! The mesh cache stores already loaded meshes and provides an interface to them.
	/** You can access it using ISceneManager::getMeshCache(). All existing
	scene managers will return a pointer to the same mesh cache, because it
	is shared between them. With this interface, it is possible to manually
	add new loaded meshes (if ISceneManager::getMesh() is not sufficient),
	to remove them and to iterate through already loaded meshes. */
	class IMeshCache : public virtual IReferenceCounted
	{
	public:

		//! Destructor
		virtual ~IMeshCache() {}

		//! Adds a mesh to the internal list of loaded meshes.
		/** Usually, ISceneManager::getMesh() is called to load a mesh
		from a file. That method searches the list of loaded meshes if
		a mesh has already been loaded and returns a pointer to if it
		is in that list and already in memory. Otherwise it loads the
		mesh. With IMeshCache::addMesh(), it is possible to pretend
		that a mesh already has been loaded. This method can be used
		for example by mesh loaders who need to load more than one mesh
		with one call. They can add additional meshes with this method
		to the scene manager. The COLLADA loader for example uses this
		method.
		\param name Name of the mesh. When calling
		ISceneManager::getMesh() with this name it will return the mesh
		set by this method.
		\param mesh Pointer to a mesh which will now be referenced by
		this name. */
		virtual void addMesh(const io::path& name, IAnimatedMesh* mesh) = 0;

		//! Removes a mesh from the cache.
		/** After loading a mesh with getMesh(), the mesh can be
		removed from the cache using this method, freeing a lot of
		memory.
		\param mesh Pointer to the mesh which shall be removed. */
		virtual void removeMesh(IAnimatedMesh* mesh) = 0;

		//! Removes a mesh from the cache.
		/** After loading a mesh with getMesh(), the mesh can be
		removed from the cache using this method, freeing a lot of
		memory.
		\param mesh Pointer to the mesh which shall be removed. */
		virtual void removeMesh(const IMesh* const mesh) = 0;

		//! Returns amount of loaded meshes in the cache.
		/** You can load new meshes into the cache using getMesh() and
		addMesh(). If you ever need to access the internal mesh cache,
		you can do this using removeMesh(), getMeshNumber(),
		and getMeshName().
		\return Number of meshes in cache. */
		virtual u32 getMeshCount() const = 0;

		//! Returns a mesh based on its name (often a filename).
		/** \deprecated Use getMeshByName() instead. */
		_IRR_DEPRECATED_ IAnimatedMesh* getMeshByFilename(const io::path& filename)
		{
			return getMeshByName(filename);
		}

		//! Get the name of a loaded mesh, if there is any. (Name is often identical to the filename).
		/** \deprecated Use getMeshName() instead. */
		_IRR_DEPRECATED_ const io::path& getMeshFilename(const IAnimatedMesh* const mesh) const
		{
			return getMeshName(mesh).getInternalName();
		}

		//! Get the name of a loaded mesh, if there is any. (Name is often identical to the filename).
		/** \deprecated Use getMeshName() instead. */
		_IRR_DEPRECATED_ const io::path& getMeshFilename(const IMesh* const mesh) const
		{
			return getMeshName(mesh).getInternalName();
		}

		//! Renames a loaded mesh.
		/** \deprecated Use renameMesh() instead. */
		_IRR_DEPRECATED_ bool setMeshFilename(IAnimatedMesh* mesh, const io::path& filename)
		{
			return renameMesh(mesh, filename);
		}

		//! Renames a loaded mesh.
		/** \deprecated Use renameMesh() instead. */
		_IRR_DEPRECATED_ bool setMeshFilename(IMesh* mesh, const io::path& filename)
		{
			return renameMesh(mesh, filename);
		}

		//! Returns a mesh based on its name.
		/** \param name Name of the mesh. Usually a filename.
		\return Pointer to the mesh or 0 if there is none with this number. */
		virtual IAnimatedMesh* getMeshByName(const io::path& name) = 0;

		//! Get the name of a loaded mesh, if there is any.
		/** \param mesh Pointer to mesh to query.
		\return The name if mesh was found and has a name, else	the path is empty. */
		virtual const io::SNamedPath& getMeshName(const IAnimatedMesh* const mesh) const = 0;

		//! Get the name of a loaded mesh, if there is any.
		/** \param mesh Pointer to mesh to query.
		\return The name if mesh was found and has a name, else	the path is empty. */
		virtual const io::SNamedPath& getMeshName(const IMesh* const mesh) const = 0;

		//! Renames a loaded mesh.
		/**\param mesh Mesh to be renamed.
		\param name New name for the mesh.
		\return True if mesh was renamed. */
		virtual bool renameMesh(IAnimatedMesh* mesh, const io::path& name) = 0;

		//! Renames a loaded mesh.
		/**\param mesh Mesh to be renamed.
		\param name New name for the mesh.
		\return True if mesh was renamed. */
		virtual bool renameMesh(IMesh* mesh, const io::path& name) = 0;

		//! Check if a mesh was already loaded.
		/** \param name Name of the mesh. Usually a filename.
		\return True if the mesh has been loaded, else false. */
		virtual bool isMeshLoaded(const io::path& name) = 0;

		//! Clears the whole mesh cache, removing all meshes.
		/** All meshes will be reloaded completely when using ISceneManager::getMesh()
		after calling this method.
		Warning: If you have pointers to meshes that were loaded with ISceneManager::getMesh()
		and you did not grab them, then they may become invalid. */
		virtual void clear() = 0;

		//! Clears all meshes that are held in the mesh cache but not used anywhere else.
		/** Warning: If you have pointers to meshes that were loaded with ISceneManager::getMesh()
		and you did not grab them, then they may become invalid. */
		virtual void clearUnusedMeshes() = 0;
	};


} // end namespace scene
} // end namespace irr

#endif

