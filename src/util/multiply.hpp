#ifndef TEXENGINE_MULTIPLY_HPP_INCLUDED
#define TEXENGINE_MULTIPLY_HPP_INCLUDED

#include "image.hpp"

using TEXEngine::Util::Image;

namespace TEXEngine {
	namespace Util {
		namespace Mutator {

// RETURN CODES:
// 0. Everything went well.
// 1. imga equals NULL.
// 2. imgb equals NULL.
// 3. Dimension mismatch.
int multiply(Image *imga, const Image *imgb);

		} /* namespace Mutator */
	} /* namespace Util */
} /* namespace TEXEngine */

#endif /* TEXENGINE_MULTIPLY_HPP_INCLUDED */
