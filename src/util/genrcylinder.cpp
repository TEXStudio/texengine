#include <vector>
#include "../math/precision.h"
#include "../math/vector.hpp"
#include "../port/vertex.h"
#include "buffer.hpp"
#include "translate.hpp"
#include "flipfaces.hpp"
#include "calcnorm.hpp"
#include "genrdisk.hpp"
#include "genrcylinder.hpp"

using TEXEngine::Math::PI;
using TEXEngine::Math::Vector3f;
using TEXEngine::Port::vertex_t;
using TEXEngine::Port::index_t;
using TEXEngine::Port::GFXAPIfloat;
using TEXEngine::Util::Mutator::translate;
using TEXEngine::Util::Mutator::flip_faces;
using TEXEngine::Util::Mutator::calc_normals;
using TEXEngine::Util::Generator::disk;
using TEXEngine::Util::Buffer;

namespace TEXEngine {
	namespace Util {
		namespace Generator {

int cylinder(Mesh *mesh, float bottom_radius, float top_radius,
			float height, unsigned int stacks, unsigned int slices,
			unsigned int b_rings, unsigned int t_rings,
			float b_inner_radius, float t_inner_radius)
{
	if(!mesh)
		return 1;

	// Stacks must be greater than 2.
	slices = slices > 2 ? slices : 3;

	// Stacks must be greater than 0.
	stacks = stacks > 0 ? stacks : 1;

	float angle_fraction = 2.0f * (float)M_PI / (float)slices;
	float offset = height / 2.0f;
/*
	# TOP DISK START
*/
	// Rings must be greater than 1.
	// The center point is treated as a ring.
	t_rings = t_rings < 2 ? 2 : t_rings;
	
	// Calculate the radius.
	// - Use absolute values
	top_radius = top_radius > 0.0f ? top_radius : -top_radius;
	t_inner_radius = t_inner_radius > 0.0f ? t_inner_radius : -t_inner_radius;

	// - In case of invalid values we set inner_radius to 0
	if (top_radius < t_inner_radius)
		t_inner_radius = 0.0f;
	
	float tf_radius = top_radius - t_inner_radius;

	std::vector<vertex_t> t_vertices;

	// Calculate all the vertices
	for (unsigned int i = 0; i < t_rings; i++) {
		float cur_radius = (float)i * (tf_radius / (float)(t_rings - 1)) + t_inner_radius;
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
			v.v = (float)i / (float)(t_rings-1);

			// We want the first vertex of every ring to be duplicated to
			// achieve good texture mapping one of the vertices will have
			// a u textcoord of 0.0 and the other one 1.0.
			if (j == 0) {
				float temp = v.u;
				v.u = 1.0f;
				t_vertices.push_back(v);
				v.u = temp;
			}

			t_vertices.push_back(v);
		}
	}

	std::vector<unsigned int> t_indices;

	for (unsigned int i = 0; i < t_rings - 1; i++) {
		for (unsigned int j = 0; j < slices + 2; j++) {
			// Indexing wizardry
			unsigned int cv = j, nv = j+1;
			if (cv == slices + 1)
				nv = 0;

			unsigned int nslices = slices + 2;

			t_indices.push_back(i * nslices + nv);
			t_indices.push_back((i + 1) * nslices + nv);
			t_indices.push_back((i + 1) * nslices + cv);

			t_indices.push_back(i * nslices + nv);
			t_indices.push_back((i + 1) * nslices + cv);
			t_indices.push_back(i * nslices + cv);
		}
	}

	// Translate to correct pivot
	for (unsigned int i = 0; i < t_vertices.size(); i++) {
		t_vertices[i].y -= (GFXAPIfloat)offset;
	}
/*
	# TOP DISK END
*/

/*
	# BOT DISK START
*/
	// Rings must be greater than 1.
	// The center point is treated as a ring.
	b_rings = b_rings < 2 ? 2 : b_rings;
	
	// Calculate the radius.
	// - Use absolute values
	bottom_radius = bottom_radius > 0.0f ? bottom_radius : -bottom_radius;
	b_inner_radius = b_inner_radius > 0.0f ? b_inner_radius : -b_inner_radius;

	// - In case of invalid values we set inner_radius to 0
	if (bottom_radius < b_inner_radius)
		b_inner_radius = 0.0f;
	
	float bf_radius = bottom_radius - b_inner_radius;

	std::vector<vertex_t> b_vertices;

	// Calculate all the vertices
	for (unsigned int i = 0; i < b_rings; i++) {
		float cur_radius = (float)i * (bf_radius / (float)(b_rings - 1)) + b_inner_radius;
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
			v.v = (float)i / (float)(b_rings-1);

			// We want the first vertex of every ring to be duplicated to
			// achieve good texture mapping one of the vertices will have
			// a u textcoord of 0.0 and the other one 1.0.
			if (j == 0) {
				float temp = v.u;
				v.u = 1.0f;
				b_vertices.push_back(v);
				v.u = temp;
			}

			b_vertices.push_back(v);
		}
	}

	std::vector<unsigned int> b_indices;

	for (unsigned int i = 0; i < b_rings - 1; i++) {
		for (unsigned int j = 0; j < slices + 2; j++) {
			// Indexing wizardry
			unsigned int cv = j, nv = j+1;
			if (cv == slices + 1)
				nv = 0;

			unsigned int nslices = slices + 2;

			b_indices.push_back(i * nslices + nv);
			b_indices.push_back((i + 1) * nslices + nv);
			b_indices.push_back((i + 1) * nslices + cv);

			b_indices.push_back(i * nslices + nv);
			b_indices.push_back((i + 1) * nslices + cv);
			b_indices.push_back(i * nslices + cv);
		}
	}

	// flip the bottom faces
	for (unsigned int i = 0; i < b_indices.size(); i+=3) {
		int temp = b_indices[i];
		b_indices[i]   = b_indices[i+2]; 
		b_indices[i+2] = temp; 
	}

	// flip the bottom normals
	// Correct the normals
	for (unsigned int i = 0; i < b_vertices.size(); i++) {
		b_vertices[i].nx *= -1.0f;
		b_vertices[i].ny *= -1.0f;
		b_vertices[i].nz *= -1.0f;
	}

	// Translate to correct pivot
	for (unsigned int i = 0; i < b_vertices.size(); i++) {
		b_vertices[i].y -= (GFXAPIfloat)offset;
	}

/*
	# BOT DISK END
*/

	std::vector<vertex_t> vertices;
	std::vector<unsigned int> indices;

	// Copy the data with corrected indices
	for (unsigned int i = 0; i < t_vertices.size(); i++)
		vertices.push_back(t_vertices[i]);
	for (unsigned int i = 0; i < b_vertices.size(); i++)
		vertices.push_back(b_vertices[i]);
	for (unsigned int i = 0; i < t_indices.size(); i++)
		indices.push_back(t_indices[i]);
	for (unsigned int i = 0; i < b_indices.size(); i++)
		indices.push_back(b_indices[i] + t_vertices.size());

	unsigned int base_idx = vertices.size();

	// Work out the body vertices
	// We will duplicate some vertices, but this is desirable behaviour.
	// The cylinder is a special object, just as the cube is.
	// We want vertices with the same position to have different normals.
	// This is crucial if we want to do phong shading.
	for (unsigned int i = 0; i < stacks + 1; i++) {
		for (unsigned int j = 0; j < slices + 1; j++) {
			vertex_t v;
			unsigned int idx_top = t_vertices.size() - (slices+1) + j;
			unsigned int idx_bottom = b_vertices.size() - (slices+1) + j;

			float base_x = b_vertices[idx_bottom].x;
			float base_z = b_vertices[idx_bottom].z;

			// Simple lerp
			float radius = 
			v.y = b_vertices[idx_bottom].y + (height / stacks) * i;
			v.x = base_x - (b_vertices[idx_bottom].x - t_vertices[idx_top].x) / (float) stacks * i;
			v.z = base_z - (b_vertices[idx_bottom].z - t_vertices[idx_top].z) / (float) stacks * i;

			v.r = 1.0f;
			v.g = 1.0f;
			v.b = 1.0f;
			v.a = 1.0f;

			v.u = (float)(j+1) / (float)(slices);
			v.v = asin(v.y / ((float)height / 2.0f)) / (float)PI + 0.5f;

			// We want the first vertex of every ring to be duplicated to
			// achieve good texture mapping one of the vertices will have
			// a u textcoord of 0.0 and the other one 1.0.
			if (j == 0) {
				float temp = v.u;
				v.u = 1.0;
				vertices.push_back(v);
				v.u = temp;
			}

			vertices.push_back(v);			
		}
	}

	// Body index assembly
	unsigned int nslices = slices + 2;

	for (unsigned int i = 0; i < stacks; i++) {
		for (unsigned int j = 0; j < slices + 2; j++) {
			// Indexing wizardry			
			unsigned int cv = j, nv = j+1;
			if (cv == slices + 1)
				nv = 0;

			indices.push_back(base_idx + (i + 1) * nslices + cv);
			indices.push_back(base_idx + (i + 1) * nslices + nv);
			indices.push_back(base_idx + i * nslices + cv);

			indices.push_back(base_idx + (i + 1) * nslices + nv);
			indices.push_back(base_idx + i * nslices + nv);
			indices.push_back(base_idx + i * nslices + cv);
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

	return 0;
}

		} /* namespace Generator */
	} /* namespace Util */
} /* namespace TEXEngine */
