#ifndef TEXENGINE_MUTIL_H_INCLUDED
#define TEXENGINE_MUTIL_H_INCLUDED

namespace TEXEngine {
	namespace Math {

#ifdef __cplusplus
	extern "C" {
#endif	/* __cplusplus */

static inline scalar_t degrees_to_radians(scalar_t degrees);
static inline scalar_t radians_to_degrees(scalar_t radians);

#ifdef __cplusplus
	}   /* extern "C" */
#endif

	} /* namespace Math */
} /* namespace TEXEngine */

#include "mutil.inl"

#endif /* TEXENGINE_MUTIL_H_INCLUDED */