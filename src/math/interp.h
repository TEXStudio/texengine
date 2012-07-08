#ifndef TEXENGINE_INTERP_H_INCLUDED
#define TEXENGINE_INTERP_H_INCLUDED

#include "precision.h"

namespace TEXEngine {
	namespace Math {
		namespace Intrp {

#ifdef __cplusplus
	extern "C" {
#endif	/* __cplusplus */

/* Simple */
static inline scalar_t step(scalar_t a, scalar_t b, scalar_t p);
static inline scalar_t linear(scalar_t a, scalar_t b, scalar_t p);
	
/* Trigonometric */
static inline scalar_t cosine(scalar_t a, scalar_t b, scalar_t p);

/* Polynomial */
static inline scalar_t acceleration(scalar_t a, scalar_t b, scalar_t p);
static inline scalar_t deceleration(scalar_t a, scalar_t b, scalar_t p);
static inline scalar_t cubic(scalar_t a, scalar_t b, scalar_t c, scalar_t d, scalar_t p);
	
/* Splines */
static inline scalar_t hermite(scalar_t t1, scalar_t a, scalar_t b, scalar_t t2, scalar_t p);
static inline scalar_t cardinal(scalar_t a, scalar_t b, scalar_t c, scalar_t d, scalar_t t, scalar_t p);
static inline scalar_t catmullrom(scalar_t a, scalar_t b, scalar_t c, scalar_t d, scalar_t p);
static inline scalar_t bezier_quadratic(scalar_t a, scalar_t b, scalar_t c, scalar_t p);
static inline scalar_t bezier_cubic(scalar_t a, scalar_t b, scalar_t c, scalar_t d, scalar_t p); /* DeCasteljau */

#ifdef __cplusplus
	}   /* extern "C" */
#endif

		} /* namespace Intrp */
	} /* namespace Math */
} /* namespace TEXEngine */

#include "interp.inl"

#endif /* TEXENGINE_INTERP_H_INCLUDED */