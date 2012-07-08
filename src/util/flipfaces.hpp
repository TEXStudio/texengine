#ifndef TEXENGINE_FLIPFACES_HPP_INCLUDED
#define TEXENGINE_FLIPFACES_HPP_INCLUDED

#include "../asset/mesh.hpp"

using TEXEngine::Asset::Mesh;

namespace TEXEngine {
	namespace Util {
		namespace Mutator {

// RETURN CODES:
// 0. Everything went well.
// 1. mesh equals NULL.
int flip_faces(Mesh *mesh);

		} /* namespace Mutator */
	} /* namespace Util */
} /* namespace TEXEngine */

#endif /* TEXENGINE_FLIPFACES_HPP_INCLUDED */