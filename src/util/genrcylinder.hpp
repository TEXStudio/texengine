#ifndef TEXENGINE_GENRCYLINDER_HPP_INCLUDED
#define TEXENGINE_GENRCYLINDER_HPP_INCLUDED

#include "../asset/mesh.hpp"

using TEXEngine::Asset::Mesh;

namespace TEXEngine {
	namespace Util {
		namespace Generator {

// RETURN CODES:
// 0. Everything went well.
// 1. mesh equals NULL.
// 2. Failed to initiate the mesh.
int cylinder(Mesh *mesh, float bottom_radius, float top_radius,
			float height, unsigned int stacks, unsigned int slices,
			unsigned int b_rings, unsigned int t_rings,
			float b_inner_radius, float t_inner_radius);

		} /* namespace Generator */
	} /* namespace Util */
} /* namespace TEXEngine */

#endif /* TEXENGINE_GENRCYLINDER_HPP_INCLUDED */