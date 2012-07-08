#include "buffer.hpp"
#include "color.hpp"
#include "alphamap.hpp"

using TEXEngine::Util::ColorRGBAf;
using TEXEngine::Util::Buffer;

namespace TEXEngine {
	namespace Util {
		namespace Mutator {

int alpha_map(Image *img, Image *alphamap)
{
	if(!img)
		return 1;

	if(!alphamap)
		return 2;

	if((img->width() != alphamap->width())
		|| (img->height() != alphamap->height()))
		return 3;

	Buffer<ColorRGBAf> *p_imgp = &(img->pixels_w());
	Buffer<ColorRGBAf> *p_alpp = &(alphamap->pixels_w());

	for (unsigned int i = 0; i < p_imgp->count(); i++) {
		float weight = 
			0.299f * p_alpp->data()[i].red +
			0.587f * p_alpp->data()[i].green +
			0.114f * p_alpp->data()[i].blue;

		p_imgp->data()[i].alpha = weight;
	}

	return 0;
}

		} /* namespace Mutator */
	} /* namespace Util */
} /* namespace TEXEngine */