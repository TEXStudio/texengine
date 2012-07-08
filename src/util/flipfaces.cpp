#include "buffer.hpp"
#include "../math/vector.hpp"
#include "../port/vertex.h"
#include "flipfaces.hpp"

using namespace TEXEngine::Math;
using namespace TEXEngine::Port;

namespace TEXEngine {
	namespace Util {
		namespace Mutator {

int flip_faces(Mesh *mesh)
{
	if(!mesh)
		return 1;

	Buffer<vertex_t> *p_meshv = &(mesh->vertices_w());
	Buffer<index_t> *p_meshi = &(mesh->indices_w());

	// Invert the faces
	for (unsigned int i = 0; i < p_meshi->count(); i+=3) {
		int temp = p_meshi->data()[i];
		p_meshi->data()[i] = p_meshi->data()[i+2]; 
		p_meshi->data()[i+2] = temp; 
	}

	// Correct the normals
	for (unsigned int i = 0; i < p_meshv->count(); i++) {
		p_meshv->data()[i].nx *= -1.0f;
		p_meshv->data()[i].ny *= -1.0f;
		p_meshv->data()[i].nz *= -1.0f;
	}

	return 0;
}

		} /* namespace Mutator */
	} /* namespace Util */
} /* namespace TEXEngine */