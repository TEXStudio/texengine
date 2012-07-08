#include <vector>
#include "../math/precision.h"
#include "../math/vector.hpp"
#include "../port/vertex.h"
#include "buffer.hpp"
#include "genrdisk.hpp"

using TEXEngine::Math::PI;
using TEXEngine::Math::Vector3f;
using TEXEngine::Port::vertex_t;
using TEXEngine::Port::index_t;
using TEXEngine::Port::GFXAPIfloat;
using TEXEngine::Util::Buffer;

namespace TEXEngine {
	namespace Util {
		namespace Generator {

int disk(Mesh *mesh, unsigned int slices, unsigned int rings,
			float outer_radius, float inner_radius)
{
	if(!mesh)
		return 1;

	// Slices must be greater than 2.
	slices = slices > 2 ? slices : 3;
	
	// Rings must be greater than 1.
	// The center point is treated as a ring.
	rings = rings < 2 ? 2 : rings;
	
	// Calculate the radius.
	// - Use absolute values
	outer_radius = outer_radius > 0.0f ? outer_radius : -outer_radius;
	inner_radius = inner_radius > 0.0f ? inner_radius : -inner_radius;
	// - In case of invalid values we set inner_radius to 0
	if (outer_radius < inner_radius)
		inner_radius = 0.0f;
	
	float radius = outer_radius - inner_radius;

	std::vector<vertex_t> vertices;

	// Calculate all the vertices
	float angle_fraction = 2.0f * (float)M_PI / (float)slices;
	for (unsigned int i = 0; i < rings; i++) {
		float cur_radius = (float)i * (radius / (float)(rings - 1)) + inner_radius;
		for (unsigned int j = 0; j < slices + 1; j++) {
			vertex_t v;
			v.x = cos(angle_fraction * (float)j) * cur_radius;
			v.z = sin(angle_fraction * (float)j) * cur_radius;
			v.y = 0.0f;

			v.nx = 0.0f;
			v.ny = 1.0f;
			v.nz = 0.0f;

			v.r = 1.0f;
			v.g = 1.0f;
			v.b = 1.0f;
			v.a = 1.0f;

			v.u = (float)j / (float)(slices-1);
			v.v = (float)i / (float)(rings-1);

			// We want the first vertex of every ring to be duplicated to
			// achieve good texture mapping one of the vertices will have
			// a u textcoord of 0.0 and the other one 1.0.
			if (j == 0) {
				float temp = v.u;
				v.u = 1.0f;
				vertices.push_back(v);
				v.u = temp;
			}

			vertices.push_back(v);
		}
	}

	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < rings - 1; i++) {
		for (unsigned int j = 0; j < slices + 2; j++) {
			// Indexing wizardry
			unsigned int cv = j, nv = j+1;
			if (cv == slices + 1)
				nv = 0;

			unsigned int nslices = slices + 2;

			indices.push_back(i * nslices + nv);
			indices.push_back((i + 1) * nslices + nv);
			indices.push_back((i + 1) * nslices + cv);

			indices.push_back(i * nslices + nv);
			indices.push_back((i + 1) * nslices + cv);
			indices.push_back(i * nslices + cv);
		}
	}

	if (mesh->init(vertices.size(), indices.size()))
		return 2;
	
	Buffer<vertex_t> *p_meshv = &(mesh->vertices_w());
	Buffer<index_t> *p_meshi = &(mesh->indices_w());

	for (unsigned int i = 0; i < vertices.size(); i++)
		p_meshv->data()[i] = vertices[i];

	for (unsigned int i = 0; i < indices.size(); i++)
		p_meshi->data()[i] = indices[i];

	return 0;
}

		} /* namespace Generator */
	} /* namespace Util */
} /* namespace TEXEngine */
