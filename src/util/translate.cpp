#include "buffer.hpp"
#include "translate.hpp"

using TEXEngine::Port::vertex_t;
using TEXEngine::Port::GFXAPIfloat;
using TEXEngine::Util::Buffer;

namespace TEXEngine {
	namespace Util {
		namespace Mutator {

int translate(Mesh *mesh, const Vector3f &v)
{
	if(!mesh)
		return 1;

	Buffer<vertex_t> *p_meshv = &(mesh->vertices_w());

	for (unsigned int i = 0; i < p_meshv->count(); i++) {
		p_meshv->data()[i].x += (GFXAPIfloat)v.x;
		p_meshv->data()[i].y += (GFXAPIfloat)v.y;
		p_meshv->data()[i].z += (GFXAPIfloat)v.z;
	}

	return 0;
}

		} /* namespace Mutator */
	} /* namespace Util */
} /* namespace TEXEngine */