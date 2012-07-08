#ifndef TEXENGINE_PRECISION_H_INCLUDED
#define TEXENGINE_PRECISION_H_INCLUDED

#include <float.h>
#include <math.h>

namespace TEXEngine {
	namespace Math {
		
#ifdef __cplusplus
	extern "C" {
#endif	/* __cplusplus */

/* Floating point precision */
#ifdef MATH_SINGLE_PRECISION
	#define SCALAR_T_MAX FLT_MAX

	typedef float scalar_t;

	#define nmath_sqrt	sqrtf
	#define nmath_abs	fabs

	#define nmath_sin	sinf
	#define nmath_cos	cosf
	#define nmath_tan	tanf
	#define nmath_asin	asinf
	#define nmath_acos	acosf
	#define nmath_atan	atanf

#else
	#define SCALAR_T_MAX DBL_MAX

	typedef double scalar_t;

	#define nmath_sqrt sqrt
	#define nmath_abs  abs
	
	#define nmath_sin	sin
	#define nmath_cos	cos
	#define nmath_tan	tan
	#define nmath_asin	asin
	#define nmath_acos	acos
	#define nmath_atan	atan

#endif /* MATH_SINGLE_PRECISION */

/* Infinity */
#ifndef INFINITY
	#define INFINITY SCALAR_T_MAX;
#endif /* INFINITY */

/* Useful scalar_t values used in comparisons */
const scalar_t FLOAT_MEDIUM = 1.e-2;   /* 0.01 */
const scalar_t FLOAT_SMALL  = 1.e-4;   /* 0.0001 */
const scalar_t FLOAT_XSMALL = 1.e-6;   /* 0.000001 */
const scalar_t FLOAT_XXSMALL= 1.e-8;   /* 0.00000001 */

#ifndef EPSILON
	#define EPSILON FLOAT_XXSMALL
#endif /* EPSILON */

/* PI */
const scalar_t PI_DOUBLE	= 6.283185307179586232;
const scalar_t PI			= 3.14159265358979323846;
const scalar_t PI_HALF		= 1.57079632679489661923;
const scalar_t PI_QUARTER	= 0.78539816339744830962;

#ifndef M_PI
    #define M_PI	    PI
#endif /* M_PI */

/* EULER_E */
const scalar_t EULER_E = 2.7182818284590452354;

#ifndef M_E
    #define M_E	EULER_E
#endif /* M_E */

const scalar_t LN2	= 0.69314718055994530942;
const scalar_t LN10	= 2.30258509299404568402;

#ifdef __cplusplus
	}   /* extern "C" */
#endif

	} /* namespace Math */
} /* namespace TEXEngine */

#endif /* TEXENGINE_PRECISION_H_INCLUDED */
