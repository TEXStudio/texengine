#include "quaternion.hpp"

namespace TEXEngine {
	namespace Math {

const Quaternion Quaternion::identity(1.0f, 0.0f, 0.0f, 0.0f);

Quaternion::Quaternion()
	: w(1.0f), x(0.0f), y(0.0f), z(0.0f)
{}

Quaternion::Quaternion(const scalar_t w_, const scalar_t x_, const scalar_t y_, const scalar_t z_)
	: w(w_), x(x_), y(y_), z(z_)
{}

Quaternion::Quaternion(const Matrix4x4f &m)
{
	from_matrix(m);
}

Quaternion::~Quaternion() 
{}

	} /* namespace Math */
} /* namespace TEXEngine */
