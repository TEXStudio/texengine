#include "../math/precision.h"
#include "../math/matrix.hpp"
#include "buffer.hpp"
#include "rotate.hpp"

using TEXEngine::Math::PI;
using TEXEngine::Math::scalar_t;
using TEXEngine::Math::Matrix3x3f;
using TEXEngine::Port::vertex_t;
using TEXEngine::Port::GFXAPIfloat;
using TEXEngine::Util::Buffer;

namespace TEXEngine {
	namespace Util {
		namespace Mutator {

int rotate(Mesh *mesh, const Vector3f &deg)
{
	if(!mesh)
		return 1;

	Matrix3x3f mat;
	float rad = (float)PI / 180.0f;
	mat.rotate(Vector3f(deg.x * rad, deg.y * rad, deg.z * rad));
	
	Buffer<vertex_t> *p_meshv = &(mesh->vertices_w());

	for (unsigned int i = 0; i < p_meshv->count(); i++) {
		Vector3f v((scalar_t)p_meshv->data()[i].x, 
			(scalar_t)p_meshv->data()[i].y, 
			(scalar_t)p_meshv->data()[i].z);

		v = mat * v;

		p_meshv->data()[i].x = (GFXAPIfloat)v.x;
		p_meshv->data()[i].y = (GFXAPIfloat)v.y;
		p_meshv->data()[i].z = (GFXAPIfloat)v.z;
	}

	return 0;
}

		} /* namespace Mutator */
	} /* namespace Util */
} /* namespace TEXEngine */