#include "buffer.hpp"
#include "chroma.hpp"

using TEXEngine::Util::ColorRGBAf;
using TEXEngine::Util::Buffer;

namespace TEXEngine {
	namespace Util {
		namespace Mutator {

int chroma_key(Image *img, const ColorRGBAf& key, const float opacity, const float threshold)
{
	if(!img)
		return 1;

	Buffer<ColorRGBAf> *p_imgp = &(img->pixels_w());

	for (unsigned int i = 0; i < p_imgp->count(); i++)
		if (p_imgp->data()[i].red <= (key.red + threshold) &&
			p_imgp->data()[i].red >= (key.red - threshold) &&
			p_imgp->data()[i].green <= (key.green + threshold) && 
			p_imgp->data()[i].green >= (key.green - threshold) &&
			p_imgp->data()[i].blue <= (key.blue + threshold) && 
			p_imgp->data()[i].blue >= (key.blue - threshold))
			p_imgp->data()[i].alpha = opacity;

	return 0;
}

		} /* namespace Mutator */
	} /* namespace Util */
} /* namespace TEXEngine */