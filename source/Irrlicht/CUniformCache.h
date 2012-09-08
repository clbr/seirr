// Copyright (C) 2012 Lauri Kasanen
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_UNIFORM_CACHE_H_INCLUDED__
#define __C_UNIFORM_CACHE_H_INCLUDED__

#include "IrrCompileConfig.h"
#include "irrTypes.h"
#include "os.h"

namespace irr
{
namespace video
{
	/* Uniform cache for OpenGL.

	Rationale:
		Dynamic array + bsearch: sort for every add, search is logN string compares.
		RB tree: search is logN string compares.
		Hash table: search is constant, one hash + one string compare. May fail.

	Using a hash table without collisions makes sense for shaders. Most shaders
	only use a few uniforms, the uber-shaders with dozens of uniforms are rare.

	So this is faster for the common case. */

	class CUniformCache: public IReferenceCounted
	{
	public:
		CUniformCache();
		~CUniformCache();

		u8 hash(const char *in);
		s32 get(const char *in);
		void add(const char *in, s32 value);

	protected:
		struct unicache {
			const char *name;
			s32 value;
		};

		unicache *cache;
	};

} // end namespace video
} // end namespace irr

#endif

