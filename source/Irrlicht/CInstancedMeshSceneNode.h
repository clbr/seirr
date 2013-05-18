// Copyright (C) 2013 Lauri Kasanen
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_INSTANCED_MESH_SCENE_NODE_H_INCLUDED__
#define __C_INSTANCED_MESH_SCENE_NODE_H_INCLUDED__

#include "IInstancedMeshSceneNode.h"
#include "IMesh.h"
#include "CEmptySceneNode.h"

namespace irr
{
namespace scene
{

	class CInstancedMeshSceneNode : public IInstancedMeshSceneNode
	{
	public:

		//! constructor
		CInstancedMeshSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
			IMesh *mesh = NULL, u32 initialInstances = 0,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));

		//! destructor
		virtual ~CInstancedMeshSceneNode();

		//! renders the node.
		virtual void render();

		//! register it
		virtual void OnRegisterSceneNode();

		virtual video::SMaterial& getMaterial(u32 i);

		virtual u32 getMaterialCount() const;

		//! returns the axis aligned bounding box of this node
		virtual const core::aabbox3d<f32>& getBoundingBox() const;

		//! Sets a new mesh
		virtual void setMesh(IMesh* mesh);

		//! Sets if the scene node should not copy the materials of the mesh but use them directly
		virtual void setReadOnlyMaterials(bool ro) { ReadOnlyMaterials = ro; }

		//! Returns type of the scene node
		virtual ESCENE_NODE_TYPE getType() const { return ESNT_MESH; }

		//! Returns if the materials are from the mesh
		virtual bool isReadOnlyMaterials() const { return ReadOnlyMaterials; }

		//! Add a new instance.
		virtual void addInstance(
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f),
			const video::SColor& color = video::SColor(255, 255, 255, 255));

		//! Get the number of instances
		virtual u32 getInstanceCount() const;

		//! Sets the position of this instance.
		/** \param num: Number of the instance to edit.
		    \param pos: New position, relative to this scene node. */
		virtual void setInstancePosition(u32 num, const core::vector3df& pos);

		//! Sets the rotation of this instance.
		/** \param num: Number of the instance to edit.
		    \param rot: New rotation, relative to this scene node. */
		virtual void setInstanceRotation(u32 num, const core::vector3df& rot);

		//! Sets the scale of this instance.
		/** \param num: Number of the instance to edit.
		    \param scale: New scale, relative to this scene node. */
		virtual void setInstanceScale(u32 num, const core::vector3df& scale);

		//! Sets the color of this instance.
		/** \param num: Number of the instance to edit.
		    \param col: New color. */
		virtual void setInstanceColor(u32 num, const video::SColor& col);

		//! Set the names of the vertex attributes
		/** By default, the color is bound to "color", and the model/world
		    matrix of each instance is bound to "model". */
		virtual void setAttributeNames(const char *color, const char *model);

		//! Clear
		virtual void clear();

		//! Remove instance
		virtual void removeInstance(u32 num);

	protected:
		void rebuildArrays();
		void rebuildBoundingBox();
		void copyMaterials();

		// Multiple inheritance causes too much problems, so just replicate
		core::array<video::SMaterial> Materials;
		core::aabbox3df Box;
		bool ReadOnlyMaterials;

		core::matrix4 OldAbsoluteTrans;

		core::array<CEmptySceneNode *> Instances;

		core::array<video::SColor> SColors;
		u32 *Colors;
		f32 *Matrices;
		char *ModelName, *ColorName;

		bool NeedsRebuild;
		bool RebuildBox;
		bool UseColors;
	};

} // end namespace scene
} // end namespace irr

#endif

