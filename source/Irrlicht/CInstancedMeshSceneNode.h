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
		virtual ~CInstancedMeshSceneNode() _IRR_OVERRIDE_;

		//! renders the node.
		virtual void render() _IRR_OVERRIDE_;

		//! register it
		virtual void OnRegisterSceneNode() _IRR_OVERRIDE_;

		virtual video::SMaterial& getMaterial(u32 i) _IRR_OVERRIDE_;

		virtual u32 getMaterialCount() const _IRR_OVERRIDE_;

		//! returns the axis aligned bounding box of this node
		virtual const core::aabbox3d<f32>& getBoundingBox() const _IRR_OVERRIDE_;

		//! Sets a new mesh
		virtual void setMesh(IMesh* mesh) _IRR_OVERRIDE_;

		//! Sets if the scene node should not copy the materials of the mesh but use them directly
		virtual void setReadOnlyMaterials(bool ro) _IRR_OVERRIDE_ { ReadOnlyMaterials = ro; }

		//! Returns type of the scene node
		virtual ESCENE_NODE_TYPE getType() const _IRR_OVERRIDE_ { return ESNT_MESH; }

		//! Returns if the materials are from the mesh
		virtual bool isReadOnlyMaterials() const _IRR_OVERRIDE_ { return ReadOnlyMaterials; }

		//! Add a new instance.
		virtual void addInstance(
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f),
			const video::SColor& color = video::SColor(255, 255, 255, 255));

		//! Get the number of instances
		virtual u32 getInstanceCount() const _IRR_OVERRIDE_;

		//! Sets the position of this instance.
		/** \param num: Number of the instance to edit.
		    \param pos: New position, relative to this scene node. */
		virtual void setInstancePosition(u32 num, const core::vector3df& pos) _IRR_OVERRIDE_;

		//! Sets the rotation of this instance.
		/** \param num: Number of the instance to edit.
		    \param rot: New rotation, relative to this scene node. */
		virtual void setInstanceRotation(u32 num, const core::vector3df& rot) _IRR_OVERRIDE_;

		//! Sets the scale of this instance.
		/** \param num: Number of the instance to edit.
		    \param scale: New scale, relative to this scene node. */
		virtual void setInstanceScale(u32 num, const core::vector3df& scale) _IRR_OVERRIDE_;

		//! Sets the color of this instance.
		/** \param num: Number of the instance to edit.
		    \param col: New color. */
		virtual void setInstanceColor(u32 num, const video::SColor& col) _IRR_OVERRIDE_;

		//! Gets the position of this instance.
		virtual const core::vector3df& getInstancePosition(u32 num) const _IRR_OVERRIDE_;

		//! Gets the rotation of this instance.
		virtual const core::vector3df& getInstanceRotation(u32 num) const _IRR_OVERRIDE_;

		//! Gets the scale of this instance.
		virtual const core::vector3df& getInstanceScale(u32 num) const _IRR_OVERRIDE_;

		//! Gets the color of this instance.
		virtual const video::SColor& getInstanceColor(u32 num) const _IRR_OVERRIDE_;

		//! Set the names of the vertex attributes
		/** By default, the color is bound to "color", and the model/world
		    matrix of each instance is bound to "model". */
		virtual void setAttributeNames(const char *color, const char *model) _IRR_OVERRIDE_;

		//! Clear
		virtual void clear() _IRR_OVERRIDE_;

		//! Remove instance
		virtual void removeInstance(u32 num) _IRR_OVERRIDE_;

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

