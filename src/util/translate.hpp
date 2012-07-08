#ifndef TEXENGINE_TRANSLATE_HPP_INCLUDED
#define TEXENGINE_TRANSLATE_HPP_INCLUDED

#include "../math/vector.hpp"
#include "../asset/mesh.hpp"

using TEXEngine::Math::Vector3f;
using TEXEngine::Asset::Mesh;

namespace TEXEngine {
	namespace Util {
		namespace Mutator {

// RETURN CODES:
// 0. Everything went well.
// 1. mesh equals NULL.
int translate(Mesh *mesh, const Vector3f &v);

		} /* namespace Mutator */
	} /* namespace Util */
} /* namespace TEXEngine */

#endif /* TEXENGINE_TRANSLATE_HPP_INCLUDED */