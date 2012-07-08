#ifndef TEXENGINE_VERTEX_H_INCLUDED
#define TEXENGINE_VERTEX_H_INCLUDED

#include "gfxapi.h"

namespace TEXEngine {
	namespace Port {

#ifdef _WIN32

struct vertex_t
{
	
		GFXAPIfloat x, y, z;		/* Position */
		GFXAPIfloat u, v;			/* Texture coordinates */
		GFXAPIfloat nx, ny, nz;		/* Normal */
		GFXAPIfloat tx, ty, tz;		/* Tangent */
		GFXAPIfloat r, g, b, a;		/* Color */
		GLubyte padding[4];			/* Pad to 64 bytes */
};

typedef GFXAPIuint index_t;

#else
	#error	"Unsupported platform."
#endif /* _WIN32 */

	} /* namespace Port */
} /* namespace TEXEngine */

#endif /* TEXENGINE_VERTEX_H_INCLUDED */