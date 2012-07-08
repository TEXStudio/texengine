#include "buffer.hpp"
#include "../math/vector.hpp"
#include "../port/vertex.h"
#include "calcnorm.hpp"

using TEXEngine::Math::Vector3f;
using TEXEngine::Port::vertex_t;
using TEXEngine::Port::index_t;
using TEXEngine::Port::GFXAPIfloat;

namespace TEXEngine {
	namespace Util {
		namespace Mutator {

int calc_normals(Mesh *mesh)
{
	if(!mesh)
		return 1;

	Buffer<vertex_t> *p_meshv = &(mesh->vertices_w());
	Buffer<index_t> *p_meshi = &(mesh->indices_w());

	// Zero out all the normals.
	for (unsigned int i = 0; i < p_meshv->count(); i++) {
		p_meshv->data()[i].nx = 0.0f;
		p_meshv->data()[i].ny = 0.0f;
		p_meshv->data()[i].nz = 0.0f;
	}

	for (unsigned int i = 0; i < p_meshi->count(); i+=3) {
		// Calculate 2 edge vectors (counter clock-wise)
		Vector3f v0(
			p_meshv->data()[p_meshi->data()[i]].x,
			p_meshv->data()[p_meshi->data()[i]].y,
			p_meshv->data()[p_meshi->data()[i]].z);

		Vector3f v1(
			p_meshv->data()[p_meshi->data()[i+1]].x,
			p_meshv->data()[p_meshi->data()[i+1]].y,
			p_meshv->data()[p_meshi->data()[i+1]].z);

		Vector3f v2(
			p_meshv->data()[p_meshi->data()[i+2]].x,
			p_meshv->data()[p_meshi->data()[i+2]].y,
			p_meshv->data()[p_meshi->data()[i+2]].z);

		Vector3f a = v2 - v0;
		Vector3f b = v1 - v0;

		// Calculate the cross product
		Vector3f fnormal = cross(b, a);

		// Modify the per-vertex normal
		p_meshv->data()[p_meshi->data()[i]].nx += (GFXAPIfloat)fnormal.x;
		p_meshv->data()[p_meshi->data()[i]].ny += (GFXAPIfloat)fnormal.y;
		p_meshv->data()[p_meshi->data()[i]].nz += (GFXAPIfloat)fnormal.z;

		p_meshv->data()[p_meshi->data()[i+1]].nx += (GFXAPIfloat)fnormal.x;
		p_meshv->data()[p_meshi->data()[i+1]].ny += (GFXAPIfloat)fnormal.y;
		p_meshv->data()[p_meshi->data()[i+1]].nz += (GFXAPIfloat)fnormal.z;

		p_meshv->data()[p_meshi->data()[i+2]].nx += (GFXAPIfloat)fnormal.x;
		p_meshv->data()[p_meshi->data()[i+2]].ny += (GFXAPIfloat)fnormal.y;
		p_meshv->data()[p_meshi->data()[i+2]].nz += (GFXAPIfloat)fnormal.z;
	}

	for (unsigned int i = 0; i < p_meshv->count(); i++) {
		Vector3f v(p_meshv->data()[i].nx, 
				   p_meshv->data()[i].ny, 
				   p_meshv->data()[i].nz);

		v.normalize();
		
		p_meshv->data()[i].nx = (GFXAPIfloat)v.x;
		p_meshv->data()[i].ny = (GFXAPIfloat)v.y;
		p_meshv->data()[i].nz = (GFXAPIfloat)v.z;
	}

	return 0;
}

		} /* namespace Mutator */
	} /* namespace Util */
} /* namespace TEXEngine */