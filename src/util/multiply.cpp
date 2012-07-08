#include "buffer.hpp"
#include "color.hpp"
#include "multiply.hpp"

using TEXEngine::Util::ColorRGBAf;
using TEXEngine::Util::Buffer;

namespace TEXEngine {
	namespace Util {
		namespace Mutator {

int multiply(Image *imga, const Image *imgb)
{
	if(!imga)
		return 1;

	if(!imgb)
		return 2;

	if((imga->width() != imgb->width())
		|| (imga->height() != imgb->height()))
		return 3;

	Buffer<ColorRGBAf> *p_imgap = &(imga->pixels_w());
	const Buffer<ColorRGBAf> *p_imgbp = &(imgb->pixels());

	for (unsigned int i = 0; i < p_imgap->count(); i++) {
		p_imgap->data()[i].red		*= p_imgbp->data()[i].red;
		p_imgap->data()[i].green	*= p_imgbp->data()[i].green;
		p_imgap->data()[i].blue		*= p_imgbp->data()[i].blue;
		p_imgap->data()[i].alpha	*= p_imgbp->data()[i].alpha;
	}

	return 0;
}

		} /* namespace Mutator */
	} /* namespace Util */
} /* namespace TEXEngine */