#ifndef TEXENGINE_MATRIX_HPP_INCLUDED
#define TEXENGINE_MATRIX_HPP_INCLUDED

#include "vector.hpp"

namespace TEXEngine {
	namespace Math {

// Forward declarations
class Matrix3x3f;
class Matrix4x4f;

class Matrix3x3f
{
	public:
		// Constructors
		Matrix3x3f();
		Matrix3x3f(	scalar_t m11, scalar_t m12, scalar_t m13,
					scalar_t m21, scalar_t m22, scalar_t m23,
					scalar_t m31, scalar_t m32, scalar_t m33);
		explicit Matrix3x3f(const Matrix4x4f &mat4);

		// Index operator
		inline scalar_t *operator [](int index);

		// Compound binary operators
		Matrix3x3f &operator +=(const Matrix3x3f &m2);
		Matrix3x3f &operator -=(const Matrix3x3f &m2);
		Matrix3x3f &operator *=(const Matrix3x3f &m2);
		//Compound scalar operators
		Matrix3x3f &operator *=(scalar_t r);

		// Reset matrix
		inline void load_identity();

		// Transformations
		void translate(const Vector2f &trans);
		void set_translation(const Vector2f &trans);

		void rotate(scalar_t angle);						/* 2d rotation */
    	void rotate(const Vector3f &euler);				/* 3d rotation with euler angles */
    	void rotate(const Vector3f &axis, scalar_t angle);	/* 3d axis/angle rotation */

		void set_rotation(scalar_t angle);
    	void set_rotation(const Vector3f &euler);
		void set_rotation(const Vector3f &axis, scalar_t angle);

		void scale(const Vector3f &vec);
		void set_scaling(const Vector3f &vec);

		// Tuple operations
		void set_column_vector(const Vector3f &vec, unsigned int index);
		void set_row_vector(const Vector3f &vec, unsigned int index);
		Vector3f get_column_vector(unsigned int index) const;
		Vector3f get_row_vector(unsigned int index) const;

		void transpose();
		Matrix3x3f transposed() const;
		scalar_t determinant() const;
		Matrix3x3f adjoint() const;
		Matrix3x3f inverse() const;

	   	static const Matrix3x3f identity;

		scalar_t data[3][3];
};

// Binary operators
Matrix3x3f operator +(const Matrix3x3f &m1, const Matrix3x3f &m2);
Matrix3x3f operator -(const Matrix3x3f &m1, const Matrix3x3f &m2);
Matrix3x3f operator *(const Matrix3x3f &m1, const Matrix3x3f &m2);
// Scalar operators
Matrix3x3f operator *(Matrix3x3f &mat, scalar_t r);
Matrix3x3f operator *(scalar_t r, const Matrix3x3f &mat);
// Vector operators
Vector3f operator *(const Matrix3x3f &mat, const Vector3f &vec);

class Matrix4x4f
{
	public:
		// Constructors
		Matrix4x4f();
		Matrix4x4f( scalar_t m11, scalar_t m12, scalar_t m13, scalar_t m14,
					scalar_t m21, scalar_t m22, scalar_t m23, scalar_t m24,
					scalar_t m31, scalar_t m32, scalar_t m33, scalar_t m34,
					scalar_t m41, scalar_t m42, scalar_t m43, scalar_t m44);
		explicit Matrix4x4f(const Matrix3x3f &mat3);

		// Index operator
		inline scalar_t *operator [](int index);

		// Compound binary operators
		Matrix4x4f &operator +=(const Matrix4x4f &m2);
		Matrix4x4f &operator -=(const Matrix4x4f &m2);
		Matrix4x4f &operator *=(const Matrix4x4f &m2);
		// Compound scalar operators
		Matrix4x4f &operator *=(scalar_t r);

		// Reset matrix
		inline void load_identity();

		// Transformations
		void translate(const Vector3f &trans);
		void set_translation(const Vector3f &trans);

		void rotate(const Vector3f &euler);           	/* 3d rotation with euler angles */
		void rotate(const Vector3f &axis, scalar_t angle); /* 3d axis/angle rotation */
		void set_rotation(const Vector3f &euler);
		void set_rotation(const Vector3f &axis, scalar_t angle);

		void scale(const Vector4f &vec);
		void set_scaling(const Vector4f &vec);
		
		// Tuple operations
 		void set_column_vector(const Vector4f &vec, unsigned int index);
		void set_row_vector(const Vector4f &vec, unsigned int index);
		Vector4f get_column_vector(unsigned int index) const;
		Vector4f get_row_vector(unsigned int index) const;

		void transpose();
		Matrix4x4f transposed() const;
		scalar_t determinant() const;
		Matrix4x4f adjoint() const;
		Matrix4x4f inverse() const;

		static const Matrix4x4f identity;

		scalar_t data[4][4];
};

// Binary operators
Matrix4x4f operator +(const Matrix4x4f &m1, const Matrix4x4f &m2);
Matrix4x4f operator -(Matrix4x4f &m1, const Matrix4x4f &m2);
Matrix4x4f operator *(const Matrix4x4f &m1, const Matrix4x4f &m2);

// Scalar operators
Matrix4x4f operator *(const Matrix4x4f &mat, scalar_t r);
Matrix4x4f operator *(scalar_t r, const Matrix4x4f &mat);
// Vector operators
Vector4f operator *(const Matrix4x4f &mat, const Vector4f &vec);

	} /* namespace Math */
} /* namespace TEXEngine */

#include "matrix.inl"

#endif /* TEXENGINE_MATRIX_HPP_INCLUDED */