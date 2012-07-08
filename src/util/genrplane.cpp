#include <vector>
#include "../math/precision.h"
#include "../math/vector.hpp"
#include "../port/vertex.h"
#include "buffer.hpp"
#include "genrplane.hpp"

using TEXEngine::Math::Vector3f;
using TEXEngine::Math::EPSILON;
using TEXEngine::Port::vertex_t;
using TEXEngine::Port::index_t;
using TEXEngine::Port::GFXAPIfloat;
using TEXEngine::Util::Buffer;

namespace TEXEngine {
	namespace Util {
		namespace Generator {

int plane(Mesh *mesh, float size, unsigned int slices, float texture_repeat)
{
	if(!mesh)
		return 1;

	// Correct the inputs
	size = size < (float)EPSILON ? 1.0f : size;
	slices = slices < 1 ? 1 : slices;
	texture_repeat = texture_repeat < (float)EPSILON ? (float)EPSILON : texture_repeat;

	std::vector<vertex_t> vertices;

	float initp = -size / 2.f;
	float step = size / (float)slices;

	for (unsigned int i = 0; i < slices + 1; i++)
		for (unsigned int j = 0; j < slices + 1; j++)
		{
			vertex_t v;

			v.x = initp + j * step;
			v.z = initp + i * step;
			v.y = 0.0f;

			v.nx = 0.0f;
			v.ny = 1.0f;
			v.nz = 0.0f;

			v.r = 1.0f;
			v.g = 1.0f;
			v.b = 1.0f;
			v.a = 1.0f;

			v.u = (float)i / (float)slices / texture_repeat;
			v.v = (float)j / (float)slices / texture_repeat;

			vertices.push_back(v);			
		}

	std::vector<index_t> indices;

	for (unsigned int i = 0; i < slices; i++)
		for (unsigned int j = 0; j < slices; j++) {
			// Indexing wizardry

			unsigned int a = j + ((slices + 1) * i);
			unsigned int b = a + 1;

			unsigned int c = j + ((slices + 1) * (i + 1));
			unsigned int d = c + 1;

			indices.push_back(a);
			indices.push_back(c);
			indices.push_back(b);

			indices.push_back(c);
			indices.push_back(d);
			indices.push_back(b);
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

	return 0;
}

		} /* namespace Generator */
	} /* namespace Util */
} /* namespace TEXEngine */