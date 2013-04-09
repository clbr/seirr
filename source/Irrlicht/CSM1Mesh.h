#ifndef __CSM1MESH_H_INCLUDED__
#define __CSM1MESH_H_INCLUDED__

#include "irrTypes.h"

namespace irr {

/*
	SM - Static Mesh, or something else.
	Always little-endian.

	The format is as follows:

	Two magic bytes "sm".
	u8 version number.
	u32 uncompressed payload size.
	Payload.


	The payload is as follows:

	u16 vertex count.
	u32 index count.
	u8 texture length.
	char array, not NULL-terminated, of length given above.
	vertices.
	indices.
*/

/*
	We need to use our own struct, due to C++ classes being
	unreliable.

	Note how the members are separated, and the struct has no
	padding.
*/
struct sm1_vert {
	float x;
	float y;
	float z;

	float nx;
	float ny;
	float nz;

	float uvx;
	float uvy;

	u8 r;
	u8 g;
	u8 b;
	u8 a;
};

enum {
	SM_VER = 1
};

}

#endif
