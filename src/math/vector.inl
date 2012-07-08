#ifndef TEXENGINE_VECTOR_INL_INCLUDED
#define TEXENGINE_VECTOR_INL_INCLUDED

#ifndef TEXENGINE_VECTOR_HPP_INCLUDED
    #error "vector.hpp must be included before vector.inl"
#endif /* TEXENGINE_VECTOR_HPP_INCLUDED */

#include <cmath>
#include "precision.h"

namespace TEXEngine {
	namespace Math {

// Vector2f
inline scalar_t Vector2f::operator [](unsigned int index) const
{
	return index ? y : x;
}

inline Vector2f& Vector2f::operator =(const Vector2f& v)
{
    x = v.x;
    y = v.y;
    return *this;
}

inline const Vector2f Vector2f::operator -()
{
	return Vector2f(-x, -y);
}

inline const Vector2f operator +(const Vector2f& v1, const Vector2f& v2)
{
	return Vector2f(v1) += v2;
}

inline const Vector2f operator -(const Vector2f& v1, const Vector2f& v2)
{
	return Vector2f(v1) -= v2;
}

inline const Vector2f operator *(const Vector2f& v1, const Vector2f& v2)
{
	return Vector2f(v1) *= v2;
}

inline const Vector2f operator /(const Vector2f& v1, const Vector2f& v2)
{
	return Vector2f(v1) /= v2;
}

inline const Vector2f operator +(const Vector2f& v, scalar_t r)
{
	return Vector2f(v) += r;
}

inline const Vector2f operator +(scalar_t r, const Vector2f& v)
{
	return Vector2f(v) += r;
}

inline const Vector2f operator -(const Vector2f& v, scalar_t r)
{
	return Vector2f(v) -= r;
}

inline const Vector2f operator *(const Vector2f& v, scalar_t r)
{
	return Vector2f(v) *= r;
}

inline const Vector2f operator *(scalar_t r, const Vector2f& v)
{
	return Vector2f(v) *= r;
}

inline const Vector2f operator /(const Vector2f& v, scalar_t r)
{
	return Vector2f(v) /= r;
}

inline Vector2f &Vector2f::operator +=(const Vector2f& v2)
{
	x += v2.x;
	y += v2.y;
	return *this;
}

inline Vector2f &Vector2f::operator -=(const Vector2f& v2)
{
	x -= v2.x;
	y -= v2.y;
	return *this;
}

inline Vector2f &Vector2f::operator *=(const Vector2f& v2)
{
	x *= v2.x;
	y *= v2.y;
	return *this;
}

inline Vector2f &Vector2f::operator /=(const Vector2f& v2)
{
	x /= v2.x;
	y /= v2.y;
	return *this;
}

inline Vector2f &Vector2f::operator +=(scalar_t r)
{
	x += r;
	y += r;
	return *this;
}

inline Vector2f &Vector2f::operator -=(scalar_t r)
{
	x -= r;
	y -= r;
	return *this;
}

inline Vector2f &Vector2f::operator *=(scalar_t r)
{
	x *= r;
	y *= r;
	return *this;
}

inline Vector2f &Vector2f::operator /=(scalar_t r)
{
	x /= r;
	y /= r;
	return *this;
}

inline bool operator ==(const Vector2f& v1, const Vector2f& v2)
{
	return (nmath_abs(v1.x - v2.x) < FLOAT_XXSMALL)
		&& (nmath_abs(v1.y - v2.y) < FLOAT_XXSMALL);
}

inline bool operator !=(const Vector2f& v1, const Vector2f& v2)
{
	return (nmath_abs(v1.x - v2.x) >= FLOAT_XXSMALL)
		&& (nmath_abs(v1.y - v2.y) >= FLOAT_XXSMALL);
}

inline scalar_t Vector2f::length() const
{
	return (scalar_t)nmath_sqrt(x*x + y*y);
}

inline scalar_t Vector2f::length_squared() const
{
	return x*x + y*y;
}

inline void Vector2f::normalize()
{
	scalar_t len = length();

	if(!len)
		return;

	x /= len;
	y /= len;
}

inline Vector2f Vector2f::normalized() const
{
	scalar_t len = length();
	return (len != 0) ? Vector2f(x / len, y / len) : *this;
}

inline void Vector2f::reflect(const Vector2f &normal)
{
	*this = reflected(normal);
}

inline Vector2f Vector2f::reflected(const Vector2f &normal) const
{
	Vector2f i = normalized();
	Vector2f n = normal.normalized();
	return (2 * dot(i, n) * n) - i;
}

inline void Vector2f::refract(const Vector2f &normal, scalar_t ior_src, scalar_t ior_dst)
{
	*this = refracted(normal, ior_src, ior_dst);
}

inline Vector2f Vector2f::refracted(const Vector2f &normal, scalar_t ior_src, scalar_t ior_dst) const
{
	Vector2f n = normal.normalized();
	Vector2f i = normalized();
	scalar_t ior = ior_src / ior_dst;

	scalar_t cos_inc = - dot(n, i);
	scalar_t radical = 1.f - ((ior * ior) * (1.f - (cos_inc * cos_inc)));

	if(radical < 0.f)
	{
		// total internal reflection
		return reflected(n);
	}

	scalar_t beta = ior * cos_inc - nmath_sqrt(radical);

	return (ior * i) + (beta * n);
}

inline scalar_t dot(const Vector2f& v1, const Vector2f& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

// Vector3f
inline scalar_t Vector3f::operator [](unsigned int index) const
{
	return index ? (index == 1 ? y : z) : x;
}

inline Vector3f& Vector3f::operator =(const Vector3f& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}

inline const Vector3f Vector3f::operator -()
{
	return Vector3f(-x, -y, -z);
}

inline const Vector3f operator +(const Vector3f& v1, const Vector3f& v2)
{
	return Vector3f(v1) += v2;
}

inline const Vector3f operator -(const Vector3f& v1, const Vector3f& v2)
{
	return Vector3f(v1) -= v2;
}

inline const Vector3f operator *(const Vector3f& v1, const Vector3f& v2)
{
	return Vector3f(v1) *= v2;
}

inline const Vector3f operator /(const Vector3f& v1, const Vector3f& v2)
{
	return Vector3f(v1) /= v2;
}

inline const Vector3f operator +(const Vector3f& v, scalar_t r)
{
	return Vector3f(v) += r;
}

inline const Vector3f operator +(scalar_t r, const Vector3f& v)
{
	return Vector3f(v) += r;
}

inline const Vector3f operator -(const Vector3f& v, scalar_t r)
{
	return Vector3f(v) -= r;
}

inline const Vector3f operator *(const Vector3f& v, scalar_t r)
{
	return Vector3f(v) *= r;
}

inline const Vector3f operator *(scalar_t r, const Vector3f& v)
{
	return Vector3f(v) *= r;
}

inline const Vector3f operator /(const Vector3f& v, scalar_t r)
{
	return Vector3f(v) /= r;
}

inline Vector3f &Vector3f::operator +=(const Vector3f& v2)
{
	x += v2.x;
	y += v2.y;
	z += v2.z;
	return *this;
}

inline Vector3f &Vector3f::operator -=(const Vector3f& v2)
{
	x -= v2.x;
	y -= v2.y;
	z -= v2.z;
	return *this;
}

inline Vector3f &Vector3f::operator *=(const Vector3f& v2)
{
	x *= v2.x;
	y *= v2.y;
	z *= v2.z;
	return *this;
}

inline Vector3f &Vector3f::operator /=(const Vector3f& v2)
{
	x /= v2.x;
	y /= v2.y;
	z /= v2.z;
	return *this;
}

inline Vector3f &Vector3f::operator +=(scalar_t r)
{
	x += r;
	y += r;
	z += r;
	return *this;
}

inline Vector3f &Vector3f::operator -=(scalar_t r)
{
	x -= r;
	y -= r;
	z -= r;
	return *this;
}

inline Vector3f &Vector3f::operator *=(scalar_t r)
{
	x *= r;
	y *= r;
	z *= r;
	return *this;
}

inline Vector3f &Vector3f::operator /=(scalar_t r)
{
	x /= r;
	y /= r;
	z /= r;
	return *this;
}

inline bool operator ==(const Vector3f& v1, const Vector3f& v2)
{
	return (nmath_abs(v1.x - v2.x) < FLOAT_XXSMALL)
		&& (nmath_abs(v1.y - v2.y) < FLOAT_XXSMALL)
		&& (nmath_abs(v1.z - v2.z) < FLOAT_XXSMALL);
}

inline bool operator !=(const Vector3f& v1, const Vector3f& v2)
{
	return (nmath_abs(v1.x - v2.x) >= FLOAT_XXSMALL)
		&& (nmath_abs(v1.y - v2.y) >= FLOAT_XXSMALL)
		&& (nmath_abs(v1.z - v2.z) >= FLOAT_XXSMALL);
}

inline scalar_t Vector3f::length() const
{
	return nmath_sqrt(x*x + y*y + z*z);
}

inline scalar_t Vector3f::length_squared() const
{
	return x*x + y*y + z*z;
}

inline void Vector3f::normalize()
{
	scalar_t len = length();

	if(!len)
		return;

	x /= len;
	y /= len;
	z /= len;
}

inline Vector3f Vector3f::normalized() const
{
	scalar_t len = length();
	return (len != 0) ? Vector3f(x / len, y / len, z / len) : *this;
}

inline void Vector3f::reflect(const Vector3f &normal)
{
	*this = reflected(normal);
}

inline Vector3f Vector3f::reflected(const Vector3f &normal) const
{
	Vector3f i = normalized();
	Vector3f n = normal.normalized();
	return (2 * dot(i, n) * n) - i;
}

inline void Vector3f::refract(const Vector3f &normal, scalar_t ior_src, scalar_t ior_dst)
{
	*this = refracted(normal, ior_src, ior_dst);
}

inline Vector3f Vector3f::refracted(const Vector3f &normal, scalar_t ior_src, scalar_t ior_dst) const
{
	Vector3f n = normal.normalized();
	Vector3f i = normalized();

	scalar_t cos_inc = dot(i, -n);

	scalar_t ior = ior_src / ior_dst;

	scalar_t radical = 1.f + ((ior * ior) * ((cos_inc * cos_inc) - 1.0f));

	if(radical < 0.f)
	{
		/* total internal reflection */
		return -reflected(n);
	}

	scalar_t beta = ior * cos_inc - nmath_sqrt(radical);

	return (ior * i) + (beta * n);
}

inline scalar_t dot(const Vector3f& v1, const Vector3f& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline Vector3f cross(const Vector3f& v1, const Vector3f& v2)
{
	return Vector3f(v1.y * v2.z - v1.z * v2.y,  v1.z * v2.x - v1.x * v2.z,  v1.x * v2.y - v1.y * v2.x);
}

// Vector4f functions
inline scalar_t Vector4f::operator [](unsigned int index) const
{
	return index ? (index == 1 ? y : (index == 2 ? z : w)) : x;
}

inline Vector4f& Vector4f::operator =(const Vector4f& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
    return *this;
}

inline const Vector4f Vector4f::operator -()
{
	return Vector4f(-x, -y, -z, -w);
}

inline const Vector4f operator +(const Vector4f& v1, const Vector4f& v2)
{
	return Vector4f(v1) += v2;
}

inline const Vector4f operator -(const Vector4f& v1, const Vector4f& v2)
{
	return Vector4f(v1) -= v2;
}

inline const Vector4f operator *(const Vector4f& v1, const Vector4f& v2)
{
	return Vector4f(v1) *= v2;
}

inline const Vector4f operator /(const Vector4f& v1, const Vector4f& v2)
{
	return Vector4f(v1) /= v2;
}

inline const Vector4f operator +(const Vector4f& v, scalar_t r)
{
	return Vector4f(v) += r;
}

inline const Vector4f operator +(scalar_t r, const Vector4f& v)
{
	return Vector4f(v) += r;
}

inline const Vector4f operator -(const Vector4f& v, scalar_t r)
{
	return Vector4f(v) -= r;
}

inline const Vector4f operator *(const Vector4f& v, scalar_t r)
{
	return Vector4f(v) *= r;
}

inline const Vector4f operator *(scalar_t r, const Vector4f& v)
{
	return Vector4f(v) *= r;
}

inline const Vector4f operator /(const Vector4f& v, scalar_t r)
{
	return Vector4f(v) /= r;
}

inline Vector4f &Vector4f::operator +=(const Vector4f& v2)
{
	x += v2.x;
	y += v2.y;
	z += v2.z;
	w += v2.w;
	return *this;
}

inline Vector4f &Vector4f::operator -=(const Vector4f& v2)
{
	x -= v2.x;
	y -= v2.y;
	z -= v2.z;
	w -= v2.w;
	return *this;
}

inline Vector4f &Vector4f::operator *=(const Vector4f& v2)
{
	x *= v2.x;
	y *= v2.y;
	z *= v2.z;
	w *= v2.w;
	return *this;
}

inline Vector4f &Vector4f::operator /=(const Vector4f& v2)
{
	x /= v2.x;
	y /= v2.y;
	z /= v2.z;
	w /= v2.w;
	return *this;
}

inline Vector4f &Vector4f::operator +=(scalar_t r)
{
	x += r;
	y += r;
	z += r;
	w += r;
	return *this;
}

inline Vector4f &Vector4f::operator -=(scalar_t r)
{
	x -= r;
	y -= r;
	z -= r;
	w -= r;
	return *this;
}

inline Vector4f &Vector4f::operator *=(scalar_t r)
{
	x *= r;
	y *= r;
	z *= r;
	w *= r;
	return *this;
}

inline Vector4f &Vector4f::operator /=(scalar_t r)
{
	x /= r;
	y /= r;
	z /= r;
	w /= r;
	return *this;
}

inline bool operator ==(const Vector4f& v1, const Vector4f& v2)
{
	return (nmath_abs(v1.x - v2.x) < FLOAT_XXSMALL)
		&& (nmath_abs(v1.y - v2.y) < FLOAT_XXSMALL)
		&& (nmath_abs(v1.z - v2.z) < FLOAT_XXSMALL)
		&& (nmath_abs(v1.w - v2.w) < FLOAT_XXSMALL);;
}

inline bool operator !=(const Vector4f& v1, const Vector4f& v2)
{
	return (nmath_abs(v1.x - v2.x) >= FLOAT_XXSMALL)
		&& (nmath_abs(v1.y - v2.y) >= FLOAT_XXSMALL)
		&& (nmath_abs(v1.z - v2.z) >= FLOAT_XXSMALL)
		&& (nmath_abs(v1.w - v2.w) >= FLOAT_XXSMALL);
}

inline scalar_t Vector4f::length() const
{
	return nmath_sqrt(x*x + y*y + z*z + w*w);
}

inline scalar_t Vector4f::length_squared() const
{
	return x*x + y*y + z*z + w*w;
}

inline void Vector4f::normalize()
{
	scalar_t len = length();

	if(!len)
		return;

	x /= len;
	y /= len;
	z /= len;
	w /= len;
}

inline Vector4f Vector4f::normalized() const
{
	scalar_t len = length();
	return (len != 0) ? Vector4f(x / len, y / len, z / len, w / len) : *this;
}

inline void Vector4f::reflect(const Vector4f &normal)
{
	*this = reflected(normal);
}

inline Vector4f Vector4f::reflected(const Vector4f &normal) const
{
	Vector4f i = normalized();
	Vector4f n = normal.normalized();
	return (2 * dot(i, n) * n) - i;
}

inline void Vector4f::refract(const Vector4f &normal, scalar_t ior_src, scalar_t ior_dst)
{
	*this = refracted(normal, ior_src, ior_dst);
}

inline Vector4f Vector4f::refracted(const Vector4f &normal, scalar_t ior_src, scalar_t ior_dst) const
{
	Vector4f n = normal.normalized();
	Vector4f i = normalized();
	scalar_t ior = ior_src / ior_dst;

	scalar_t cos_inc = - dot(n, i);
	scalar_t radical = 1.f - ((ior * ior) * (1.f - (cos_inc * cos_inc)));

	if(radical < 0.f)
	{
		/* total internal reflection */
		return reflected(n);
	}

	scalar_t beta = ior * cos_inc - nmath_sqrt(radical);

	return (ior * i) + (beta * n);
}

inline scalar_t dot(const Vector4f& v1, const Vector4f& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

	} /* namespace Math */
} /* namespace TEXEngine */

#endif /* TEXENGINE_VECTOR_INL_INCLUDED */