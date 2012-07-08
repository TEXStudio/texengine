#include "buffer.hpp"
#include "color.hpp"
#include "xortex.hpp"

using TEXEngine::Util::ColorRGBAf;
using TEXEngine::Util::Buffer;

namespace TEXEngine 
{
	namespace Util 
	{
		namespace Generator 
		{

			int xortex(Image *img, unsigned int width, unsigned int height)
			{
				if(!img)
				{
					return 1;
				}

				if(!width || !height)
				{
					return 2;
				}

				if(img->init(width, height))
				{
					return 3;
				}

				for(unsigned int j = 0; j < height; j++)
				{
					for(unsigned int i = 0; i < width; i++) 
					{
						float val = (float)((char)i ^ (char)j) / 255.0f;
						unsigned int idx = j * width + i;
						img->pixels().data()[idx] = ColorRGBAf(val, val, val, 1.0f);
					}
				}

				return 0;
			}

		} /* namespace Generator */
	} /* namespace Util */
} /* namespace TEXEngine */