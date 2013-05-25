// Copyright (C) 2002-2010 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_PARTICLE_SYSTEM_SCENE_NODE_H_INCLUDED__
#define __C_PARTICLE_SYSTEM_SCENE_NODE_H_INCLUDED__

#include "IParticleSystemSceneNode.h"
#include "irrArray.h"
#include "irrList.h"
#include "SMeshBuffer.h"

namespace irr
{
namespace scene
{

//! A particle system scene node.
/** A scene node controlling a particle system. The behavior of the particles
can be controlled by setting the right particle emitters and affectors.
*/
class CParticleSystemSceneNode : public IParticleSystemSceneNode
{
public:

	//! constructor
	CParticleSystemSceneNode(bool createDefaultEmitter,
		ISceneNode* parent, ISceneManager* mgr, s32 id,
		const core::vector3df& position,
		const core::vector3df& rotation,
		const core::vector3df& scale);

	//! destructor
	virtual ~CParticleSystemSceneNode() _IRR_OVERRIDE_;

	//! Gets the particle emitter, which creates the particles.
	virtual IParticleEmitter* getEmitter() _IRR_OVERRIDE_;

	//! Sets the particle emitter, which creates the particles.
	virtual void setEmitter(IParticleEmitter* emitter) _IRR_OVERRIDE_;

	//! Adds new particle affector to the particle system.
	virtual void addAffector(IParticleAffector* affector) _IRR_OVERRIDE_;

	//! Removes all particle affectors in the particle system.
	virtual void removeAllAffectors() _IRR_OVERRIDE_;

	//! Returns the material based on the zero based index i.
	virtual video::SMaterial& getMaterial(u32 i) _IRR_OVERRIDE_;

	//! Returns amount of materials used by this scene node.
	virtual u32 getMaterialCount() const _IRR_OVERRIDE_;

	//! pre render event
	virtual void OnRegisterSceneNode() _IRR_OVERRIDE_;

	//! render
	virtual void render() _IRR_OVERRIDE_;

	//! returns the axis aligned bounding box of this node
	virtual const core::aabbox3d<f32>& getBoundingBox() const _IRR_OVERRIDE_;

	//! Creates a particle emitter for an animated mesh scene node
	virtual IParticleAnimatedMeshSceneNodeEmitter* createAnimatedMeshSceneNodeEmitter(
		scene::IAnimatedMeshSceneNode* node, bool useNormalDirection = true,
		const core::vector3df& direction = core::vector3df(0.0f,0.03f,0.0f),
		f32 normalDirectionModifier = 100.0f, s32 mbNumber = -1,
		bool everyMeshVertex = false, u32 minParticlesPerSecond = 5,
		u32 maxParticlesPerSecond = 10,
		const video::SColor& minStartColor = video::SColor(255,0,0,0),
		const video::SColor& maxStartColor = video::SColor(255,255,255,255),
		u32 lifeTimeMin = 2000, u32 lifeTimeMax = 4000,
		s32 maxAngleDegrees = 0,
		const core::dimension2df& minStartSize = core::dimension2df(5.0f,5.0f),
		const core::dimension2df& maxStartSize = core::dimension2df(5.0f,5.0f) ) _IRR_OVERRIDE_;

	//! Creates a box particle emitter.
	virtual IParticleBoxEmitter* createBoxEmitter(
		const core::aabbox3df& box = core::aabbox3d<f32>(-10,0,-10,5,30,10),
		const core::vector3df& direction = core::vector3df(0.0f,0.03f,0.0f),
		u32 minParticlesPerSecond = 5,
		u32 maxParticlesPerSecond = 10,
		const video::SColor& minStartColor = video::SColor(255,0,0,0),
		const video::SColor& maxStartColor = video::SColor(255,255,255,255),
		u32 lifeTimeMin=2000, u32 lifeTimeMax=4000,
		s32 maxAngleDegrees=0,
		const core::dimension2df& minStartSize = core::dimension2df(5.0f,5.0f),
		const core::dimension2df& maxStartSize = core::dimension2df(5.0f,5.0f) ) _IRR_OVERRIDE_;

	//! Creates a particle emitter for emitting from a cylinder
	virtual IParticleCylinderEmitter* createCylinderEmitter(
		const core::vector3df& center, f32 radius,
		const core::vector3df& normal, f32 length,
		bool outlineOnly = false, const core::vector3df& direction = core::vector3df(0.0f,0.5f,0.0f),
		u32 minParticlesPerSecond = 5, u32 maxParticlesPersSecond = 10,
		const video::SColor& minStartColor = video::SColor(255,0,0,0),
		const video::SColor& maxStartColor = video::SColor(255,255,255,255),
		u32 lifeTimeMin = 2000, u32 lifeTimeMax = 4000,
		s32 maxAngleDegrees = 0,
		const core::dimension2df& minStartSize = core::dimension2df(5.0f,5.0f),
		const core::dimension2df& maxStartSize = core::dimension2df(5.0f,5.0f) ) _IRR_OVERRIDE_;

	//! Creates a mesh particle emitter.
	virtual IParticleMeshEmitter* createMeshEmitter(
		scene::IMesh* mesh, bool useNormalDirection = true,
		const core::vector3df& direction = core::vector3df(0.0f,0.03f,0.0f),
		f32 normalDirectionModifier = 100.0f, s32 mbNumber = -1,
		bool everyMeshVertex = false,
		u32 minParticlesPerSecond = 5,
		u32 maxParticlesPerSecond = 10,
		const video::SColor& minStartColor = video::SColor(255,0,0,0),
		const video::SColor& maxStartColor = video::SColor(255,255,255,255),
		u32 lifeTimeMin = 2000, u32 lifeTimeMax = 4000,
		s32 maxAngleDegrees = 0,
		const core::dimension2df& minStartSize = core::dimension2df(5.0f,5.0f),
		const core::dimension2df& maxStartSize = core::dimension2df(5.0f,5.0f) ) _IRR_OVERRIDE_;

	//! Creates a point particle emitter.
	virtual IParticlePointEmitter* createPointEmitter(
		const core::vector3df& direction = core::vector3df(0.0f,0.03f,0.0f),
		u32 minParticlesPerSecond = 5,
		u32 maxParticlesPerSecond = 10,
		const video::SColor& minStartColor = video::SColor(255,0,0,0),
		const video::SColor& maxStartColor = video::SColor(255,255,255,255),
		u32 lifeTimeMin=2000, u32 lifeTimeMax=4000,
		s32 maxAngleDegrees=0,
		const core::dimension2df& minStartSize = core::dimension2df(5.0f,5.0f),
		const core::dimension2df& maxStartSize = core::dimension2df(5.0f,5.0f) ) _IRR_OVERRIDE_;

	//! Creates a ring particle emitter.
	virtual IParticleRingEmitter* createRingEmitter(
		const core::vector3df& center, f32 radius, f32 ringThickness,
		const core::vector3df& direction = core::vector3df(0.0f,0.03f,0.0f),
		u32 minParticlesPerSecond = 5,
		u32 maxParticlesPerSecond = 10,
		const video::SColor& minStartColor = video::SColor(255,0,0,0),
		const video::SColor& maxStartColor = video::SColor(255,255,255,255),
		u32 lifeTimeMin=2000, u32 lifeTimeMax=4000,
		s32 maxAngleDegrees=0,
		const core::dimension2df& minStartSize = core::dimension2df(5.0f,5.0f),
		const core::dimension2df& maxStartSize = core::dimension2df(5.0f,5.0f) ) _IRR_OVERRIDE_;

	//! Creates a sphere particle emitter.
	virtual IParticleSphereEmitter* createSphereEmitter(
		const core::vector3df& center, f32 radius,
		const core::vector3df& direction = core::vector3df(0.0f,0.03f,0.0f),
		u32 minParticlesPerSecond = 5,
		u32 maxParticlesPerSecond = 10,
		const video::SColor& minStartColor = video::SColor(255,0,0,0),
		const video::SColor& maxStartColor = video::SColor(255,255,255,255),
		u32 lifeTimeMin=2000, u32 lifeTimeMax=4000,
		s32 maxAngleDegrees=0,
		const core::dimension2df& minStartSize = core::dimension2df(5.0f,5.0f),
		const core::dimension2df& maxStartSize = core::dimension2df(5.0f,5.0f) ) _IRR_OVERRIDE_;

	//! Creates a point attraction affector. This affector modifies the positions of the
	//! particles and attracts them to a specified point at a specified speed per second.
	virtual IParticleAttractionAffector* createAttractionAffector(
		const core::vector3df& point, f32 speed = 1.0f, bool attract = true,
		bool affectX = true, bool affectY = true, bool affectZ = true) _IRR_OVERRIDE_;

	//! Creates a scale particle affector.
	virtual IParticleAffector* createScaleParticleAffector(const core::dimension2df& scaleTo = core::dimension2df(1.0f, 1.0f)) _IRR_OVERRIDE_;

	//! Creates a fade out particle affector.
	virtual IParticleFadeOutAffector* createFadeOutParticleAffector(
		const video::SColor& targetColor = video::SColor(0,0,0,0),
		u32 timeNeededToFadeOut = 1000) _IRR_OVERRIDE_;

	//! Creates a gravity affector.
	virtual IParticleGravityAffector* createGravityAffector(
		const core::vector3df& gravity = core::vector3df(0.0f,-0.03f,0.0f),
		u32 timeForceLost = 1000) _IRR_OVERRIDE_;

	//! Creates a rotation affector. This affector rotates the particles
	//! around a specified pivot point. The speed is in Degrees per second.
	virtual IParticleRotationAffector* createRotationAffector(
		const core::vector3df& speed = core::vector3df(5.0f,5.0f,5.0f),
		const core::vector3df& pivotPoint = core::vector3df(0.0f,0.0f,0.0f) ) _IRR_OVERRIDE_;

	//! Sets the size of all particles.
	virtual void setParticleSize(
		const core::dimension2d<f32> &size = core::dimension2d<f32>(5.0f, 5.0f)) _IRR_OVERRIDE_;

	//! Sets if the particles should be global. If they are, the particles are affected by
	//! the movement of the particle system scene node too, otherwise they completely
	//! ignore it. Default is true.
	virtual void setParticlesAreGlobal(bool global=true) _IRR_OVERRIDE_;

	//! Writes attributes of the scene node.
	virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const _IRR_OVERRIDE_;

	//! Reads attributes of the scene node.
	virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0) _IRR_OVERRIDE_;

	//! Returns type of the scene node
	virtual ESCENE_NODE_TYPE getType() const _IRR_OVERRIDE_ { return ESNT_PARTICLE_SYSTEM; }

private:

	void doParticleSystem(u32 time);
	void reallocateBuffers();

	core::list<IParticleAffector*> AffectorList;
	IParticleEmitter* Emitter;
	core::array<SParticle> Particles;
	core::dimension2d<f32> ParticleSize;
	u32 LastEmitTime;
	s32 MaxParticles;

	SMeshBuffer* Buffer;

	enum E_PARTICLES_PRIMITIVE
	{
		EPP_POINT=0,
		EPP_BILLBOARD,
		EPP_POINTSPRITE
	};
	E_PARTICLES_PRIMITIVE ParticlePrimitive;

	bool ParticlesAreGlobal;
};

} // end namespace scene
} // end namespace irr


#endif

