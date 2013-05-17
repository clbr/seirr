// Copyright (C) 2013 Lauri Kasanen
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_INSTANCED_MESH_SCENE_NODE_H_INCLUDED__
#define __I_INSTANCED_MESH_SCENE_NODE_H_INCLUDED__

#include "IMeshSceneNode.h"

namespace irr
{
namespace scene
{
	//! Scene node to render many instances of a static mesh.
	/** This scene node allows you to use geometry instancing on
	    supported hardware (dx9 cards and up). This means you can
	    render a crowd/forest/etc with just one draw call.

	    You will need to write a shader to use this node, instancing
	    is not supported using the fixed pipeline.

	    You may use the colors to send any custom per-instance data.
	    */
	class IInstancedMeshSceneNode : public IMeshSceneNode
	{
	public:

		//! Constructor
		IInstancedMeshSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
			IMesh *mesh = NULL, u32 initialInstances = 0,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f))
			: IMeshSceneNode(parent, mgr, id, position, rotation, scale) {}

		//! Destructor
		virtual ~IInstancedMeshSceneNode() {}

		//! Add a new instance.
		virtual void addInstance(
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f),
			const video::SColor& color = video::SColor(255, 255, 255, 255)) = 0;

		//! Get the number of instances
		virtual u32 getInstanceCount() const = 0;

		//! Sets the position of this instance.
		/** \param num: Number of the instance to edit.
		    \param pos: New position, relative to this scene node. */
		virtual void setInstancePosition(u32 num, const core::vector3df& pos) = 0;

		//! Sets the rotation of this instance.
		/** \param num: Number of the instance to edit.
		    \param rot: New rotation, relative to this scene node. */
		virtual void setInstanceRotation(u32 num, const core::vector3df& rot) = 0;

		//! Sets the scale of this instance.
		/** \param num: Number of the instance to edit.
		    \param scale: New scale, relative to this scene node. */
		virtual void setInstanceScale(u32 num, const core::vector3df& scale) = 0;

		//! Sets the color of this instance.
		/** \param num: Number of the instance to edit.
		    \param col: New color. */
		virtual void setInstanceColor(u32 num, const video::SColor& col) = 0;

		//! Set the names of the vertex attributes
		/** By default, the color is bound to "color", and the model/world
		    matrix of each instance is bound to "model". */
		virtual void setAttributeNames(const char *color, const char *model) = 0;

		//! Remove all instances
		virtual void clear() = 0;

		//! Remove this instance
		virtual void removeInstance(u32 num) = 0;
	};

} // end namespace scene
} // end namespace irr

#endif

