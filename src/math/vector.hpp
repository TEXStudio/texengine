#ifndef TEXENGINE_VECTOR_HPP_INCLUDED
#define TEXENGINE_VECTOR_HPP_INCLUDED

#include <cstdio>
#include <cmath>

#include "precision.h"

namespace TEXEngine {
	namespace Math {

// Forward declarations
class Vector2f;
class Vector3f;
class Vector4f;

/*
    2D VECTOR
*/
class Vector2f
{
    public:
        // Constructors
		explicit Vector2f(scalar_t aX = 0.0, scalar_t aY = 0.0);
        explicit Vector2f(const Vector3f &v);
        explicit Vector2f(const Vector4f &v);

        // Array subscript
        inline scalar_t operator [](unsigned int index) const;

		// Arithmetic operators
		// Unary operator
		inline const Vector2f operator -();
		// Compound assignment operators
		// - Binary
		inline Vector2f &operator +=(const Vector2f &v2);
		inline Vector2f &operator -=(const Vector2f &v2);
		inline Vector2f &operator *=(const Vector2f &v2);
		inline Vector2f &operator /=(const Vector2f &v2);
		// - scalar
		inline Vector2f &operator +=(scalar_t r);
		inline Vector2f &operator -=(scalar_t r);
		inline Vector2f &operator *=(scalar_t r);
		inline Vector2f &operator /=(scalar_t r);

        // Assignment operator
        inline Vector2f &operator =(const Vector2f &v);

        // Vector member functions
        // - Length
        inline scalar_t length() const;
        inline scalar_t length_squared() const;
        // - Normalization
        inline void normalize();
        inline Vector2f normalized() const;
        // - Reflection / Refraction
        inline void reflect(const Vector2f &normal);
        inline Vector2f reflected(const Vector2f &normal) const;
        inline void refract(const Vector2f &normal, scalar_t ior_src, scalar_t ior_dst);
        inline Vector2f refracted(const Vector2f &normal, scalar_t ior_src, scalar_t ior_dst) const;

        scalar_t x, y;
};

// - Binary
inline const Vector2f operator +(const Vector2f &v1, const Vector2f &v2);
inline const Vector2f operator -(const Vector2f &v1, const Vector2f &v2);
inline const Vector2f operator *(const Vector2f &v1, const Vector2f &v2);
inline const Vector2f operator /(const Vector2f &v1, const Vector2f &v2);
// - scalar
inline const Vector2f operator +(scalar_t r, const Vector2f &v);
inline const Vector2f operator +(const Vector2f &v, scalar_t r);
inline const Vector2f operator -(const Vector2f &v, scalar_t r);
inline const Vector2f operator *(scalar_t r, const Vector2f &v);
inline const Vector2f operator *(const Vector2f &v, scalar_t r);
inline const Vector2f operator /(const Vector2f &v, scalar_t r);
// Comparison operations
inline bool operator ==(const Vector2f &v1, const Vector2f &v2);
inline bool operator !=(const Vector2f &v1, const Vector2f &v2);

// Dot
inline scalar_t dot(const Vector2f &v1, const Vector2f &v2);

/*
    3D VECTOR
*/
class Vector3f
{
    public:
        // Constructors
        explicit Vector3f(scalar_t aX = 0.0, scalar_t aY = 0.0, scalar_t aZ = 0.0);
        explicit Vector3f(const Vector2f &v);
        explicit Vector3f(const Vector4f &v);

        // Array subscript
        inline scalar_t operator [](unsigned int index) const;

		// Arithmetic operators
		// Unary operator
		inline const Vector3f operator -();
		// Compound assignment operators
		// - Binary
		inline Vector3f &operator +=(const Vector3f &v2);
		inline Vector3f &operator -=(const Vector3f &v2);
		inline Vector3f &operator *=(const Vector3f &v2);
		inline Vector3f &operator /=(const Vector3f &v2);
		// - r
		inline Vector3f &operator +=(scalar_t r);
		inline Vector3f &operator -=(scalar_t r);
		inline Vector3f &operator *=(scalar_t r);
		inline Vector3f &operator /=(scalar_t r);

        // Assignment operator
        inline Vector3f &operator =(const Vector3f &v);

        // Vector member functions
        // - Length
        inline scalar_t length() const;
        inline scalar_t length_squared() const;
        // - Normalization
        inline void normalize();
        inline Vector3f normalized() const;
        // - Reflection / Refraction
        inline void reflect(const Vector3f &normal);
        inline Vector3f reflected(const Vector3f &normal) const;
        inline void refract(const Vector3f &normal, scalar_t ior_src, scalar_t ior_dst);
        inline Vector3f refracted(const Vector3f &normal, scalar_t ior_src, scalar_t ior_dst) const;

        scalar_t x, y, z;
};

// - Binary
inline const Vector3f operator +(const Vector3f &v1, const Vector3f &v2);
inline const Vector3f operator -(const Vector3f &v1, const Vector3f &v2);
inline const Vector3f operator *(const Vector3f &v1, const Vector3f &v2);
inline const Vector3f operator /(const Vector3f &v1, const Vector3f &v2);
// - scalar
inline const Vector3f operator +(scalar_t r, const Vector3f &v);
inline const Vector3f operator +(const Vector3f &v, scalar_t r);
inline const Vector3f operator -(const Vector3f &v, scalar_t r);
inline const Vector3f operator *(scalar_t r, const Vector3f &v);
inline const Vector3f operator *(const Vector3f &v, scalar_t r);
inline const Vector3f operator /(const Vector3f &v, scalar_t r);

// Comparison operations
inline bool operator ==(const Vector3f &v1, const Vector3f &v2);
inline bool operator !=(const Vector3f &v1, const Vector3f &v2);

inline scalar_t dot(const Vector3f &v1, const Vector3f &v2);
inline Vector3f cross(const Vector3f &v1, const Vector3f &v2);

/*
    4D VECTOR
*/
class Vector4f
{
    public:
        // Constructors
        explicit Vector4f(scalar_t aX = 0.0, scalar_t aY = 0.0, scalar_t aZ = 0.0, scalar_t aW = 0.0);
        explicit Vector4f(const Vector2f &v);
        explicit Vector4f(const Vector3f &v);

        // Array subscript
        inline scalar_t operator [](unsigned int index) const;

        // Assignment operator
        inline Vector4f &operator =(const Vector4f &v);

		// Arithmetic operators
		// Unary operator
		inline const Vector4f operator -();
		// Compound assignment operators
		// - Binary
		inline Vector4f &operator +=(const Vector4f &v2);
		inline Vector4f &operator -=(const Vector4f &v2);
		inline Vector4f &operator *=(const Vector4f &v2);
		inline Vector4f &operator /=(const Vector4f &v2);
		// - scalar
		inline Vector4f &operator +=(scalar_t r);
		inline Vector4f &operator -=(scalar_t r);
		inline Vector4f &operator *=(scalar_t r);
		inline Vector4f &operator /=(scalar_t r);

        /* Vector member functions */
        /* - Length */
        inline scalar_t length() const;
        inline scalar_t length_squared() const;
        /* - Normalization */
        inline void normalize();
        inline Vector4f normalized() const;
        /* - Reflection / Refraction */
        inline void reflect(const Vector4f &normal);
        inline Vector4f reflected(const Vector4f &normal) const;
        inline void refract(const Vector4f &normal, scalar_t ior_src, scalar_t ior_dst);
        inline Vector4f refracted(const Vector4f &normal, scalar_t ior_src, scalar_t ior_dst) const;

        scalar_t x, y, z, w;
};

// - Binary
inline const Vector4f operator +(Vector4f &v1, const Vector4f &v2);
inline const Vector4f operator -(const Vector4f &v1, const Vector4f &v2);
inline const Vector4f operator *(const Vector4f &v1, const Vector4f &v2);
inline const Vector4f operator /(const Vector4f &v1, const Vector4f &v2);
// - scalar
inline const Vector4f operator +(scalar_t r, const Vector4f &v);
inline const Vector4f operator +(const Vector4f &v, scalar_t r);
inline const Vector4f operator -(const Vector4f &v, scalar_t r);
inline const Vector4f operator *(scalar_t r, const Vector4f &v);
inline const Vector4f operator *(const Vector4f &v, scalar_t r);
inline const Vector4f operator /(const Vector4f &v, scalar_t r);
// Comparison operations */
inline bool operator ==(const Vector4f &v1, const Vector4f &v2);
inline bool operator !=(const Vector4f &v1, const Vector4f &v2);

inline scalar_t dot(const Vector4f &v1, const Vector4f &v2);

	} /* namespace Math */
} /* namespace TEXEngine */

#include "vector.inl"

#endif /* TEXENGINE_VECTOR_HPP_INCLUDED */
