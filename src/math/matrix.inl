#ifndef TEXENGINE_MATRIX_INL_INCLUDED
#define TEXENGINE_MATRIX_INL_INCLUDED

#ifndef TEXENGINE_MATRIX_HPP_INCLUDED
    #error "matrix.hpp must be included before matrix.inl"
#endif /* TEXENGINE_MATRIX_HPP_INCLUDED */

#include <cstring>

namespace TEXEngine {
	namespace Math {

inline scalar_t *Matrix3x3f::operator [](int index)
{
	scalar_t *m = &data[0][0];
    return &m[index < 9 ? index : 8];
}

inline void Matrix3x3f::load_identity()
{
    memcpy(data, identity.data, 9 * sizeof(scalar_t));
}

inline scalar_t *Matrix4x4f::operator [](int index)
{
	scalar_t *m = &data[0][0];
    return &m[index < 16 ? index : 15];
}

inline void Matrix4x4f::load_identity()
{
    memcpy(data, identity.data, 16 * sizeof(scalar_t));
}

	} /* namespace Math */
} /* namespace TEXEngine */

#endif /* TEXENGINE_MATRIX_INL_INCLUDED */