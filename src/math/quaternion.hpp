#ifndef TEXENGINE_QUATERNION_HPP_INCLUDED
#define TEXENGINE_QUATERNION_HPP_INCLUDED

#include "precision.h"
#include "matrix.hpp"

namespace TEXEngine {
	namespace Math {

class Quaternion
{
	public:
		Quaternion();
		Quaternion(const scalar_t w_, const scalar_t x_, const scalar_t y_, const scalar_t z_);
		explicit Quaternion(const Matrix4x4f &m);
		~Quaternion();

		inline bool operator==(const Quaternion &rhs) const;
		inline bool operator!=(const Quaternion &rhs) const;

		inline Quaternion &operator+=(const Quaternion &rhs);
		inline Quaternion &operator-=(const Quaternion &rhs);
		inline Quaternion &operator*=(const Quaternion &rhs);
		inline Quaternion &operator*=(const scalar_t scalar);
		inline Quaternion &operator/=(const scalar_t scalar);

		inline void from_matrix(const Matrix4x4f &m);
		inline Matrix4x4f to_matrix() const;

		inline void load_identity();
		inline Quaternion conjugate() const;
		inline Quaternion inverse() const;
		inline scalar_t magnitude() const;
		inline void normalize();
		inline void set(scalar_t w_, scalar_t x_, scalar_t y_, scalar_t z_);
		
		static const Quaternion identity;
		scalar_t w, x, y, z;
};


inline Quaternion operator+(const Quaternion &lhs, const Quaternion &rhs);
inline Quaternion operator-(const Quaternion &lhs, const Quaternion &rhs);
inline Quaternion operator*(const Quaternion &lhs, const Quaternion &rhs);
inline Quaternion operator*(const Quaternion &lhs, const scalar_t scalar);
inline Quaternion operator*(const scalar_t scalar, const Quaternion &rhs);
inline Quaternion operator/(const Quaternion &lhs, const scalar_t scalar);

	} /* namespace Math */
} /* namespace TEXEngine */

#include "quaternion.inl"

#endif /* TEXENGINE_QUATERNION_HPP_INCLUDED */