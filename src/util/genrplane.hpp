#ifndef TEXENGINE_GENRPLANE_HPP_INCLUDED
#define TEXENGINE_GENRPLANE_HPP_INCLUDED

#include "../asset/mesh.hpp"

using TEXEngine::Asset::Mesh;

namespace TEXEngine {
	namespace Util {
		namespace Generator {

// RETURN CODES:
// 0. Everything went well.
// 1. mesh equals NULL.
// 2. Failed to initiate the mesh.
int plane(Mesh *mesh, float size, unsigned int slices, float texture_repeat);

		} /* namespace Generator */
	} /* namespace Util */
} /* namespace TEXEngine */

#endif /* TEXENGINE_GENRPLANE_HPP_INCLUDED */