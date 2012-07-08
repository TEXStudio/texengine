#ifndef TEXENGINE_INTERP_INL_INCLUDED
#define TEXENGINE_INTERP_INL_INCLUDED

#ifndef LIBNMATH_INTERP_H_INCLUDED
	#error "interp.h must be included before interp.inl"
#endif /* LIBNMATH_INTERP_H_INCLUDED */

#include <math.h>

namespace TEXEngine {
	namespace Math {
		namespace Intrp {

#ifdef __cplusplus
	extern "C" {
#endif	/* __cplusplus */

/* Simple */
static inline scalar_t step(scalar_t a, scalar_t b, scalar_t p)
{    
	return (p < .5f) ? a : b;
}

static inline scalar_t linear(scalar_t a, scalar_t b, scalar_t p)
{
	return  (a * (1.f - p)) + (b * p);
}
	
/* Trigonometric */
static inline scalar_t cosine(scalar_t a, scalar_t b, scalar_t p)
{    
	/*
		First we turn the p value into an angle to sample from the
		cosine wave and sample from the wave but converting the
		scale run between 0 and 1 instead of the wave's usual -1 to 1.
		Lastly we perform a normal linear interpolation, but using the
		value from the cosine wave instead of the value of the given p
	*/
	
	scalar_t p2 = (1.f - cos(p * PI)) / 2;
	return(a * (1 - p2) + b * p2);
}

/* Polynomial */
static inline scalar_t acceleration(scalar_t a, scalar_t b, scalar_t p)
{
	scalar_t np = p * p;
	return  (a * (1.f - np)) + (b * np);
}

static inline scalar_t deceleration(scalar_t a, scalar_t b, scalar_t p)
{
	scalar_t op = 1 - p;
	scalar_t np = 1 - (op * op);
	return  (a * (1.f - np)) + (b * np);
}

static inline scalar_t cubic(scalar_t a, scalar_t b, scalar_t c, scalar_t d, scalar_t p)
{
	scalar_t P = (d - c) - (a - b);
	scalar_t Q = (a - b) - P;
	scalar_t R = c - a;
	scalar_t S = b;
	
	float p2 = p * p;
	float p3 = p2 * p;
	return (P * p3) + (Q * p2) + (R * p) + S;
}
	
/* Splines */
static inline scalar_t hermite(scalar_t t1, scalar_t a, scalar_t b, scalar_t t2, scalar_t p)
{
	float p2 = p * p;
	float p3 = p2 * p;

	float h1 = (2 * p3) - (3 * p2) + 1; /* calculate basis function 1 */
	float h2 = 1 - h1;                  /* calculate basis function 2 */
	float h3 = p3 - (2 * p2) + p;       /* calculate basis function 3 */
	float h4 = p3 - p2;					/* calculate basis function 4 */

	/* Multiply and sum all functions together */
	return (h1 * a) + (h2 * b) + (h3 * t1) + (h4 * t2);
}

static inline scalar_t cardinal(scalar_t a, scalar_t b, scalar_t c, scalar_t d, scalar_t t, scalar_t p)
{
	scalar_t t2 = t * t;
    scalar_t p3 = p * p * p;

    return t * ((2 * b) + ((c - a) * t) + 
			(((2 * a) - (5 * b) + (4 * c) - d) * t2) +
			(((3 * b) - (3 * c) + d - a) * p3));
}

static inline scalar_t catmullrom(scalar_t a, scalar_t b, scalar_t c, scalar_t d, scalar_t p)
{
    /* Note: Catmull-Roms are cardinals with a tension of 0.5 */
    scalar_t P = - (.5f * a) + (1.5f * b) - (1.5f * c) + (0.5f * d);
    scalar_t Q = a - (2.5 * b) + (2 * c) - (0.5 * d);
    scalar_t R = ( c - a ) * .5f;
    scalar_t S = b;

   	float p2 = p * p;
    float p3 = p2 * p;

    return (P * p3) + (Q * p2) + (R * p) + S;
}

static inline scalar_t bezier_quadratic(scalar_t a, scalar_t b, scalar_t c, scalar_t p)
{
	scalar_t ab, bc;
    ab = interp_linear(a, b, p);
    bc = interp_linear(b, c, p);
    return interp_linear( ab, bc, p);
}

/* DeCasteljau method */
static inline scalar_t bezier_cubic(scalar_t a, scalar_t b, scalar_t c, scalar_t d, scalar_t p)
{
    scalar_t ab, bc, cd, abc, bcd;
    ab = interp_linear(a, b, p);
    bc = interp_linear(b, c, p);
    cd = interp_linear(c, d, p);
    abc = interp_linear(ab, bc, p);
    bcd = interp_linear(bc, cd, p);
    return interp_linear( abc, bcd, p);
}

#ifdef __cplusplus
	}   /* extern "C" */
#endif

		} /* namespace Intrp */
	} /* namespace Math */
} /* namespace TEXEngine */

#endif /* TEXENGINE_INTERP_INL_INCLUDED */