#ifndef TEXENGINE_CHROMA_HPP_INCLUDED
#define TEXENGINE_CHROMA_HPP_INCLUDED

#include "../math/precision.h"
#include "color.hpp"
#include "image.hpp"

using TEXEngine::Util::Image;
using TEXEngine::Math::FLOAT_XXSMALL;

namespace TEXEngine {
	namespace Util {
		namespace Mutator {

// RETURN CODES:
// 0. Everything went well.
// 1. img equals NULL.
int chroma_key(Image *img, const ColorRGBAf& key, const float opacity = 0, const float threshold = FLOAT_XXSMALL);

		} /* namespace Mutator */
	} /* namespace Util */
} /* namespace TEXEngine */

#endif /* TEXENGINE_CHROMA_HPP_INCLUDED */
