#ifndef TEXENGINE_GENRSPHERE_HPP_INCLUDED
#define TEXENGINE_GENRSPHERE_HPP_INCLUDED

#include "../asset/mesh.hpp"

using TEXEngine::Asset::Mesh;

namespace TEXEngine {
	namespace Util {
		namespace Generator {

// RETURN CODES:
// 0. Everything went well.
// 1. mesh equals NULL.
// 2. Failed to initiate the mesh.
int sphere(Mesh *mesh, float radius, unsigned int slices, unsigned int stacks);

		} /* namespace Generator */
	} /* namespace Util */
} /* namespace TEXEngine */

#endif /* TEXENGINE_GENRSPHERE_HPP_INCLUDED */