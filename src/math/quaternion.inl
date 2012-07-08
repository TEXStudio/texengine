#ifndef TEXENGINE_QUATERNION_INL_INCLUDED
#define TEXENGINE_QUATERNION_INL_INCLUDED

#include "mutil.h"

#ifndef TEXENGINE_QUATERNION_HPP_INCLUDED
    #error "quaternion.hpp must be included before quaternion.inl"
#endif /* TEXENGINE_QUATERNION_HPP_INCLUDED */

namespace TEXEngine {
	namespace Math {

inline bool Quaternion::operator==(const Quaternion &rhs) const
{
	// deltas
	scalar_t dw = w - rhs.w;
	scalar_t dx = x - rhs.x;
	scalar_t dy = y - rhs.y;
	scalar_t dz = z - rhs.z;

	// Absolutes
	if (dw < 0) dw = -dw;
	if (dx < 0) dx = -dx;
	if (dy < 0) dy = -dy;
	if (dz < 0) dz = -dz;

	return
		dw < EPSILON &&
		dx < EPSILON &&
		dy < EPSILON &&
		dz < EPSILON;
}

inline bool Quaternion::operator!=(const Quaternion &rhs) const
{
	return !(*this == rhs);
}

inline Quaternion &Quaternion::operator+=(const Quaternion &rhs)
{
	w += rhs.w;
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

inline Quaternion &Quaternion::operator-=(const Quaternion &rhs)
{
	w -= rhs.w;
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

inline Quaternion &Quaternion::operator*=(const Quaternion &rhs)
{
	// Rotations are applied in left to right order.
    Quaternion tmp(
        (w * rhs.w) - (x * rhs.x) - (y * rhs.y) - (z * rhs.z),
        (w * rhs.x) + (x * rhs.w) - (y * rhs.z) + (z * rhs.y),
        (w * rhs.y) + (x * rhs.z) + (y * rhs.w) - (z * rhs.x),
        (w * rhs.z) - (x * rhs.y) + (y * rhs.x) + (z * rhs.w));

	*this = tmp;
	return *this;
}

inline Quaternion &Quaternion::operator*=(const scalar_t scalar)
{
	w *= scalar;
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

inline Quaternion &Quaternion::operator/=(const scalar_t scalar)
{
	w /= scalar;
	x /= scalar;
	y /= scalar;
	z /= scalar;
	return *this;
}

inline Quaternion operator+(const Quaternion &lhs, const Quaternion &rhs)
{
    return Quaternion(lhs) += rhs;
}

inline Quaternion operator-(const Quaternion &lhs, const Quaternion &rhs)
{
	return Quaternion(lhs) -= rhs;
}

inline Quaternion operator*(const Quaternion &lhs, const Quaternion &rhs)
{
	return Quaternion(lhs) *= rhs;
}

inline Quaternion operator*(const Quaternion &lhs, const scalar_t scalar)
{
	return Quaternion(lhs) *= scalar;
}

inline Quaternion operator*(const scalar_t scalar, const Quaternion &rhs)
{
	return Quaternion(rhs) *= scalar;
}

inline Quaternion operator/(const Quaternion &lhs, const scalar_t scalar)
{
	return Quaternion(lhs) /= scalar;
}

inline void Quaternion::load_identity()
{
    w = 1.0f;
	x = y = z = 0.0f;
}

inline Quaternion Quaternion::conjugate() const
{
    return Quaternion(w, -x, -y, -z);
}

inline Quaternion Quaternion::inverse() const
{
    return conjugate() / magnitude();
}

inline scalar_t Quaternion::magnitude() const
{
    return nmath_sqrt(w * w + x * x + y * y + z * z);
}

inline void Quaternion::normalize()
{
    scalar_t magn = magnitude();
    w /= magn, x /= magn, y /= magn, z /= magn;
}

inline void Quaternion::set(scalar_t w_, scalar_t x_, scalar_t y_, scalar_t z_)
{
    w = w_, x = x_, y = y_, z = z_;
}

inline void Quaternion::from_matrix(const Matrix4x4f &m)
{
	// Creates a quaternion from a rotation matrix. 
    // The algorithm used is from Allan and Mark Watt's "Advanced 
    // Animation and Rendering Techniques" (ACM Press 1992).

    scalar_t s = 0.0f;
    scalar_t q[4] = {0.0f};
    scalar_t trace = m.data[0][0] + m.data[1][1] + m.data[2][2];

    if (trace > 0.0f) {
        s = nmath_sqrt(trace + 1.0f);
        q[3] = s * 0.5f;
        s = 0.5f / s;
        q[0] = (m.data[1][2] - m.data[2][1]) * s;
        q[1] = (m.data[2][0] - m.data[0][2]) * s;
        q[2] = (m.data[0][1] - m.data[1][0]) * s;

    } else {
        int nxt[3] = {1, 2, 0};
        int i = 0, j = 0, k = 0;

        if (m.data[1][1] > m.data[0][0])
            i = 1;

        if (m.data[2][2] > m.data[i][i])
            i = 2;

        j = nxt[i];
        k = nxt[j];
        s = nmath_sqrt((m.data[i][i] - (m.data[j][j] + m.data[k][k])) + 1.0f);

        q[i] = s * 0.5f;
        s = 0.5f / s;
        q[3] = (m.data[j][k] - m.data[k][j]) * s;
        q[j] = (m.data[i][j] + m.data[j][i]) * s;
        q[k] = (m.data[i][k] + m.data[k][i]) * s;
    }

    x = q[0], y = q[1], z = q[2], w = q[3];
}

inline Matrix4x4f Quaternion::to_matrix() const
{
	/*
		Convert the quaternion to a rotation matrix.
    
		  | 1 - 2(y^2 + z^2)	2(xy + wz)			2(xz - wy)			0  |
		  | 2(xy - wz)			1 - 2(x^2 + z^2)	2(yz + wx)			0  |
		  | 2(xz + wy)			2(yz - wx)			1 - 2(x^2 + y^2)	0  |
		  | 0					0					0					1  |
	*/

    scalar_t x2 = x + x; 
    scalar_t y2 = y + y; 
    scalar_t z2 = z + z;
    scalar_t xx = x * x2;
    scalar_t xy = x * y2;
    scalar_t xz = x * z2;
    scalar_t yy = y * y2;
    scalar_t yz = y * z2;
    scalar_t zz = z * z2;
    scalar_t wx = w * x2;
    scalar_t wy = w * y2;
    scalar_t wz = w * z2;

    Matrix4x4f m;

	m.data[0][0] = 1.0f - (yy + zz);
    m.data[0][1] = xy + wz;
    m.data[0][2] = xz - wy;
    m.data[0][3] = 0.0f;

    m.data[1][0] = xy - wz;
    m.data[1][1] = 1.0f - (xx + zz);
    m.data[1][2] = yz + wx;
    m.data[1][3] = 0.0f;

    m.data[2][0] = xz + wy;
    m.data[2][1] = yz - wx;
    m.data[2][2] = 1.0f - (xx + yy);
    m.data[2][3] = 0.0f;

    m.data[3][0] = 0.0f;
    m.data[3][1] = 0.0f;
    m.data[3][2] = 0.0f;
    m.data[3][3] = 1.0f;

    return m;
}

	} /* namespace Math */
} /* namespace TEXEngine */

#endif /* TEXENGINE_QUATERNION_INL_INCLUDED */