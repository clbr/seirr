// Copyright (C) 2012 Lauri Kasanen
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CUniformCache.h"

namespace irr
{
namespace video
{

CUniformCache::CUniformCache()
{
	// C++ lacks a "allocate and zero" facility.
	// Calling a constructor N times is not efficient.
	cache = (unicache *) calloc(256, sizeof(unicache));

	if (!cache)
		os::Printer::log("CUniformCache: Failed to allocate memory.", ELL_ERROR);
}

CUniformCache::~CUniformCache()
{
	free(cache);
}

// Position-dependent fast hash:
// hash(ab) != hash(ba)
u8 CUniformCache::hash(const char *in)
{
	u16 sum = 0;
	u8 pos = 2;

	for (; *in; in++, pos++) {
		sum += *in * pos;
	}

	sum += pos;

	const u8 out = sum % 256;

	return out;
}

s32 CUniformCache::get(const char *in)
{
	const u8 hashed = hash(in);

	// Nothing at this address?
	if (!cache[hashed].name)
		return -1;

	// Something at this address, but not me?
	if (strcmp(in, cache[hashed].name) != 0)
	{
//		printf("Collision between %s and %s\n", in, cache[hashed].name);
		return -1;
	}

	return cache[hashed].value;
}

void CUniformCache::add(const char *in, s32 value)
{
	const u8 hashed = hash(in);

	if (cache[hashed].name)
		return;

	cache[hashed].name = in;
	cache[hashed].value = value;
}

} // end namespace video
} // end namespace irr

