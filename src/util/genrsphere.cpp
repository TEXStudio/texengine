#include <vector>
#include "../math/precision.h"
#include "../math/vector.hpp"
#include "../port/vertex.h"
#include "buffer.hpp"
#include "calcnorm.hpp"
#include "genrsphere.hpp"

using TEXEngine::Math::Vector3f;
using TEXEngine::Math::PI;
using TEXEngine::Port::vertex_t;
using TEXEngine::Port::index_t;
using TEXEngine::Port::GFXAPIfloat;
using TEXEngine::Util::Mutator::calc_normals;

namespace TEXEngine {
	namespace Util {
		namespace Generator {

int sphere(Mesh *mesh, float radius, unsigned int slices, unsigned int stacks)
{
	if(!mesh)
		return 1;

	// Radius must be greater than 0
	radius = radius > 0 ? radius : 1.0f;

	// Stacks must be greater than 2.
	slices = slices > 2 ? slices : 3;

	// Stacks must be greater than 0.
	stacks = stacks > 2 ? stacks : 3;

	std::vector<vertex_t> vertices;
	std::vector<index_t> indices;

	// Calculate all the body vertices
	float angle_fraction = 2.0f * (float)PI / (float)slices;

	for (unsigned int i = 0; i <= stacks; i++) {
		float cur_radius = sin((float)M_PI * i / stacks) * radius;
		float cur_y = cos((float)M_PI * i / stacks) * radius;
		for (unsigned int j = 0; j < slices; j++) {
			vertex_t v;
			v.x = cos(angle_fraction * (float)j) * cur_radius;
			v.z = sin(angle_fraction * (float)j) * cur_radius;
			v.y = cur_y;

			Vector3f norm(v.x, v.y, v.z);
			norm.normalize();
			v.nx = (GFXAPIfloat)norm.x;
			v.ny = (GFXAPIfloat)norm.y;
			v.nz = (GFXAPIfloat)norm.z;

			v.r = 1.0f;
			v.g = 1.0f;
			v.b = 1.0f;
			v.a = 1.0f;

			v.u = asin(v.nx) / (float)M_PI + 0.5f;
			v.v = asin(v.ny) / (float)M_PI + 0.5f;

			vertices.push_back(v);
		}
	}

	// Body index assembly
	for (unsigned int i = 0; i < stacks; i++) {
		for (unsigned int j = 0; j < slices; j++) {
			// Indexing wizardry
			unsigned int cv = j, nv = j+1;
			if (cv == slices - 1)
				nv = 0;
			
			indices.push_back(i * slices + cv);
			indices.push_back((i + 1) * slices + nv);
			indices.push_back((i + 1) * slices + cv);
				
			indices.push_back(i * slices + cv);
			indices.push_back(i * slices + nv);
			indices.push_back((i + 1) * slices + nv);
		}
	}

	// Copy the data
	if (mesh->init(vertices.size(), indices.size()))
		return 2;

	Buffer<vertex_t> *p_meshv = &(mesh->vertices_w());
	Buffer<index_t> *p_meshi = &(mesh->indices_w());

	for (unsigned int i = 0; i < vertices.size(); i++)
		p_meshv->data()[i] = vertices[i];

	for (unsigned int i = 0; i < indices.size(); i++)
		p_meshi->data()[i] = indices[i];

	calc_normals(mesh);

	return 0;
}

		} /* namespace Generator */
	} /* namespace Util */
} /* namespace TEXEngine */