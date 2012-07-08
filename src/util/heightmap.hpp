#ifndef TEXENGINE_HEIGHTMAP_HPP_INCLUDED
#define TEXENGINE_HEIGHTMAP_HPP_INCLUDED

#include "../asset/mesh.hpp"

using TEXEngine::Asset::Mesh;

namespace TEXEngine {
	namespace Util {
		namespace Mutator {

// RETURN CODES:
// 0. Everything went well.
// 1. mesh equal to NULL.
// 2. heightmap equal to NULL.
// 3. failed to load the heightmap.
int heightmap(Mesh *mesh, const char *heightmap, const float base = 0.0f, const float multiplier = 1.0f);

		} /* namespace Mutator */
	} /* namespace Util */
} /* namespace TEXEngine */

#endif /* TEXENGINE_HEIGHTMAP_HPP_INCLUDED */