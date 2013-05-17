// Copyright (C) 2013 Lauri Kasanen
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CInstancedMeshSceneNode.h"
#include "IVideoDriver.h"
#include "ISceneManager.h"
#include "S3DVertex.h"
#include "ICameraSceneNode.h"
#include "IMeshCache.h"
#include "IAnimatedMesh.h"
#include "IMaterialRenderer.h"
#include "SViewFrustum.h"

namespace irr
{
namespace scene
{

using namespace core;
using namespace video;

//! constructor
CInstancedMeshSceneNode::CInstancedMeshSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
		IMesh *mesh, u32 initialInstances,
		const vector3df& position, const vector3df& rotation,
		const vector3df& scale)
: IInstancedMeshSceneNode(parent, mgr, id, mesh, initialInstances, position, rotation, scale),
	Mesh(0), ReadOnlyMaterials(false), Colors(0), Matrices(0),
	ModelName(0), ColorName(0), NeedsRebuild(false), RebuildBox(false)
{
	#ifdef _DEBUG
	setDebugName("CInstancedMeshSceneNode");
	#endif

	ModelName = strdup("model");
	ColorName = strdup("color");

	OldAbsoluteTrans = AbsoluteTransformation;

	setMesh(mesh);

	Instances.reallocate(initialInstances);
	SColors.reallocate(initialInstances);

	u32 i;
	for (i = 0; i < initialInstances; i++)
	{
		Instances.push_back(new CEmptySceneNode(this, mgr, -1));
		SColors.push_back(SColor(0xffffffff));
	}

	RebuildBox = true;
}


//! destructor
CInstancedMeshSceneNode::~CInstancedMeshSceneNode()
{
	if (Mesh)
		Mesh->drop();

	free(Colors);
	free(Matrices);
	free(ModelName);
	free(ColorName);
}

void CInstancedMeshSceneNode::setMesh(IMesh *mesh)
{
	if (mesh)
	{
		mesh->grab();
		if (Mesh)
			Mesh->drop();

		Mesh = mesh;
		copyMaterials();
	}
}

void CInstancedMeshSceneNode::copyMaterials()
{
	Materials.clear();

	if (Mesh)
	{
		SMaterial mat;

		const u32 max = Mesh->getMeshBufferCount();
		for (u32 i = 0; i < max; ++i)
		{
			IMeshBuffer* mb = Mesh->getMeshBuffer(i);
			if (mb)
				mat = mb->getMaterial();

			Materials.push_back(mat);
		}
	}
}

//! frame
void CInstancedMeshSceneNode::OnRegisterSceneNode()
{
	if (!IsVisible)
		return;

	if (RebuildBox)
	{
		rebuildBoundingBox();
		RebuildBox = false;
	}

	// because this node supports rendering of mixed mode meshes consisting of
	// transparent and solid material at the same time, we need to go through all
	// materials, check of what type they are and register this node for the right
	// render pass according to that.

	IVideoDriver* driver = SceneManager->getVideoDriver();

	int transparentCount = 0;
	int solidCount = 0;

	// count transparent and solid materials in this scene node
	if (ReadOnlyMaterials && Mesh)
	{
		// count mesh materials

		const u32 max = Mesh->getMeshBufferCount();
		for (u32 i=0; i < max; ++i)
		{
			IMeshBuffer* mb = Mesh->getMeshBuffer(i);
			IMaterialRenderer* rnd = mb ? driver->getMaterialRenderer(mb->getMaterial().MaterialType) : 0;

			if (rnd && rnd->isTransparent())
				++transparentCount;
			else
				++solidCount;

			if (solidCount && transparentCount)
				break;
		}
	}
	else
	{
		// count copied materials

		const u32 max = Materials.size();
		for (u32 i=0; i < max; ++i)
		{
			IMaterialRenderer* rnd =
				driver->getMaterialRenderer(Materials[i].MaterialType);

			if (rnd && rnd->isTransparent())
				++transparentCount;
			else
				++solidCount;

			if (solidCount && transparentCount)
				break;
		}
	}

	// register according to material types counted

	if (solidCount)
		SceneManager->registerNodeForRendering(this, ESNRP_SOLID);

	if (transparentCount)
		SceneManager->registerNodeForRendering(this, ESNRP_TRANSPARENT);

	ISceneNode::OnRegisterSceneNode();
}

SMaterial& CInstancedMeshSceneNode::getMaterial(u32 i)
{
	if (Mesh && ReadOnlyMaterials && i < Mesh->getMeshBufferCount())
	{
		return Mesh->getMeshBuffer(i)->getMaterial();
	}

	if (i >= Materials.size())
		return ISceneNode::getMaterial(i);

	return Materials[i];
}


//! returns amount of materials used by this scene node.
u32 CInstancedMeshSceneNode::getMaterialCount() const
{
	if (Mesh && ReadOnlyMaterials)
		return Mesh->getMeshBufferCount();

	return Materials.size();
}


//! renders the node.
void CInstancedMeshSceneNode::render()
{
	IVideoDriver* driver = SceneManager->getVideoDriver();

	if (!Mesh || !driver)
		return;

	if (NeedsRebuild || OldAbsoluteTrans != AbsoluteTransformation)
		rebuildArrays();
	OldAbsoluteTrans = AbsoluteTransformation;

	const bool isTransparentPass =
		SceneManager->getSceneNodeRenderPass() == ESNRP_TRANSPARENT;

	driver->setTransform(ETS_WORLD, AbsoluteTransformation);

	const u32 mbcount = Mesh->getMeshBufferCount();
	const u32 len = Instances.size();

	u32 i;
	for (i = 0; i < mbcount; ++i)
	{
		IMeshBuffer* mb = Mesh->getMeshBuffer(i);
		if (!mb)
			continue;

		const SMaterial& mat = ReadOnlyMaterials ? mb->getMaterial() : Materials[i];

		const IMaterialRenderer* rnd = driver->getMaterialRenderer(mat.MaterialType);
		const bool transparent = (rnd && rnd->isTransparent());

#define GL_UNSIGNED_BYTE 0x1401
#define GL_FLOAT 0x1406
		driver->setCustomVertexAttribute(mat.MaterialType,
				ColorName, GL_UNSIGNED_BYTE, 1, true,
				4, Colors);
		driver->setCustomVertexAttribute(mat.MaterialType,
				ModelName, GL_FLOAT, 1, false,
				16*4, Matrices);


		// only render transparent buffer if this is the transparent render pass
		// and solid only in solid pass
		if (transparent == isTransparentPass)
		{
			driver->setMaterial(mat);
			driver->drawMeshBuffer(mb, len);
		}
	}
}


//! returns the axis aligned bounding box of this node
const aabbox3d<f32>& CInstancedMeshSceneNode::getBoundingBox() const
{
	return Box;
}

//! Add a new instance.
void CInstancedMeshSceneNode::addInstance(
	const vector3df& pos,
	const vector3df& rot,
	const vector3df& scale,
	const SColor& col)
{
	Instances.push_back(new CEmptySceneNode(this, SceneManager, -1));

	ISceneNode * const cur = Instances.getLast();
	cur->setPosition(pos);
	cur->setRotation(rot);
	cur->setScale(scale);

	SColors.push_back(col);

	NeedsRebuild = true;
	RebuildBox = true;
}

//! Get the number of instances
u32 CInstancedMeshSceneNode::getInstanceCount() const
{
	return Instances.size();
}

//! Sets the position of this instance.
void CInstancedMeshSceneNode::setInstancePosition(u32 num, const vector3df& pos)
{
	if (num >= Instances.size())
		return;

	Instances[num]->setPosition(pos);

	NeedsRebuild = true;
	RebuildBox = true;
}

//! Sets the rotation of this instance.
void CInstancedMeshSceneNode::setInstanceRotation(u32 num, const vector3df& rot)
{
	if (num >= Instances.size())
		return;

	Instances[num]->setRotation(rot);

	NeedsRebuild = true;
	RebuildBox = true;
}

//! Sets the scale of this instance.
void CInstancedMeshSceneNode::setInstanceScale(u32 num, const vector3df& scale)
{
	if (num >= Instances.size())
		return;

	Instances[num]->setScale(scale);

	NeedsRebuild = true;
	RebuildBox = true;
}

//! Sets the color of this instance.
void CInstancedMeshSceneNode::setInstanceColor(u32 num, const SColor& col)
{
	if (num >= Instances.size())
		return;

	SColors[num] = col;

	NeedsRebuild = true;
}

void CInstancedMeshSceneNode::rebuildBoundingBox()
{
	u32 i;
	const u32 len = Instances.size();
	if (!len || !Mesh)
		return;


	// Box, for culling of the whole thing
	const aabbox3df &mbox = Mesh->getBoundingBox();

	{
		aabbox3df tmpbox = mbox;
		const matrix4 &mat = Instances[0]->getRelativeTransformation();
		mat.transformBoxEx(tmpbox);

		Box = tmpbox;
	}

	for (i = 1; i < len; i++)
	{
		aabbox3df tmpbox = mbox;
		const matrix4 &mat = Instances[i]->getRelativeTransformation();
		mat.transformBoxEx(tmpbox);

		Box.addInternalBox(tmpbox);
	}


}

void CInstancedMeshSceneNode::rebuildArrays()
{
	free(Colors);
	free(Matrices);

	const u32 len = Instances.size();
	if (!len || !Mesh)
		return;

	updateAbsolutePosition();

	Colors = (u32 *) calloc(len, sizeof(u32));
	Matrices = (f32 *) calloc(len * 16, sizeof(f32));

	// Colors
	u32 i;
	for (i = 0; i < len; i++)
	{
		// We can count on anything supporting instancing supporting BGRA.
		// Both are dx9 features.
		Colors[i] = SColors[i].color;
	}

	// Matrices
	for (i = 0; i < len; i++)
	{
		Instances[i]->updateAbsolutePosition();

		// Instance matrices are relative to this node.
		const matrix4 &mat = Instances[i]->getAbsoluteTransformation();

		memcpy(&Matrices[16 * i], mat.pointer(), sizeof(f32) * 16);
	}

	// Done
	NeedsRebuild = false;
}

void CInstancedMeshSceneNode::setAttributeNames(const char *color, const char *model)
{
	if (color)
	{
		free(ColorName);
		ColorName = strdup(color);
	}

	if (model)
	{
		free(ModelName);
		ModelName = strdup(model);
	}
}

void CInstancedMeshSceneNode::clear()
{
	Instances.clear();
	Box.reset(0, 0, 0);
}

void CInstancedMeshSceneNode::removeInstance(u32 num)
{
	if (num >= Instances.size())
		return;

	Instances.erase(num);

	NeedsRebuild = true;
	RebuildBox = true;
}

} // end namespace scene
} // end namespace irr

