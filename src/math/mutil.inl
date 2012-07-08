#ifndef TEXENGINE_MUTIL_INL_INCLUDED
#define TEXENGINE_MUTIL_INL_INCLUDED

#include "precision.h"

#ifndef TEXENGINE_MUTIL_H_INCLUDED
    #error "mutil.h must be included before mutil.inl"
#endif /* TEXENGINE_MUTIL_H_INCLUDED */

namespace TEXEngine {
	namespace Math {

#ifdef __cplusplus
	extern "C" {
#endif	/* __cplusplus */

static inline scalar_t degrees_to_radians(scalar_t degrees)
{
	return (degrees * PI) / 180.0f;
}

static inline scalar_t radians_to_degrees(scalar_t radians)
{
	return (radians * 180.0f) / PI;
}

#ifdef __cplusplus
	}   /* extern "C" */
#endif

	} /* namespace Math */
} /* namespace TEXEngine */

#endif /* TEXENGINE_MUTIL_INL_INCLUDED */