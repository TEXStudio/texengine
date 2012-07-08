#ifndef TEXENGINE_ALPHAMAP_HPP_INCLUDED
#define TEXENGINE_ALPHAMAP_HPP_INCLUDED

#include "image.hpp"

using TEXEngine::Util::Image;

namespace TEXEngine {
	namespace Util {
		namespace Mutator {

// RETURN CODES:
// 0. Everything went well.
// 1. img equal to NULL.
// 2. alphamap equal to NULL;
// 3. Dimension mismatch.
int alpha_map(Image *img, Image *alphamap);

		} /* namespace Mutator */
	} /* namespace Util */
} /* namespace TEXEngine */

#endif /* TEXENGINE_ALPHAMAP_HPP_INCLUDED */