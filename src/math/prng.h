#ifndef TEXENGINE_PRNG_H_INCLUDED
#define TEXENGINE_PRNG_H_INCLUDED

#include "precision.h"

namespace TEXEngine {
	namespace Math {
		namespace PRNG {

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

/* Generate a random number between min and max using the C built-in PRNG in uniform manner */
static inline scalar_t prng_c(scalar_t a, scalar_t b);

/* Multiply with carry method by George Marsaglia */
static inline scalar_t prng_multiplyWithCarry(scalar_t a, scalar_t b);

#ifdef __cplusplus
}   /* extern "C" */
#endif

		} /* namespace PRNG */
	} /* namespace Math */
} /* namespace TEXEngine */

#include "prng.inl"

#endif /* TEXENGINE_PRNG_H_INCLUDED */
