#ifndef TEXENGINE_GENRDISK_HPP_INCLUDED
#define TEXENGINE_GENRDISK_HPP_INCLUDED

#include "../asset/mesh.hpp"

using TEXEngine::Asset::Mesh;

namespace TEXEngine {
	namespace Util {
		namespace Generator {

// RETURN CODES:
// 0. Everything went well.
// 1. mesh equals NULL.
// 2. Failed to initiate the mesh.
int disk(Mesh *mesh, unsigned int slices, unsigned int rings,
			float outer_radius, float inner_radius);

		} /* namespace Generator */
	} /* namespace Util */
} /* namespace TEXEngine */

#endif /* TEXENGINE_GENRDISK_HPP_INCLUDED */