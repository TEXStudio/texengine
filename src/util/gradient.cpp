#include "buffer.hpp"
#include "color.hpp"
#include "gradient.hpp"

using TEXEngine::Util::TEXENG_GRADIENT_TYPE;
using TEXEngine::Util::Buffer;
using TEXEngine::Math::dot;
using TEXEngine::Math::scalar_t;

namespace TEXEngine {
	namespace Util {
		namespace Generator {

int gradient(Image *img, const unsigned int width, const unsigned int height,
	const ColorRGBAf &col1, const ColorRGBAf &col2, const Vector2f &v, 
	const bool alpha, const TEXENG_GRADIENT_TYPE type)
{
	if(!img)
		return 1;

	if(!width || !height)
		return 2;

	if(img->init(width, height))
		return 3;

	Vector2f d = v.normalized();

	for(unsigned int j = 0; j < height; j++)
		for(unsigned int i = 0; i < width; i++) {
			
			unsigned int idx = j * width + i;
			
			Vector2f cur(i, j);
			
			ColorRGBAf fin, dif = col2 - col1;
			
			switch(type) {
				case TEXENG_GRADIENT_PERPIXEL:
				{
					float sc = (float)dot(v.normalized(), cur.normalized());
					fin = col1 + ColorRGBAf(sc * dif.red, 
						sc * dif.green, sc * dif.blue, alpha ? sc * dif.alpha : 1.0f);
					break;
				}

				case TEXENG_GRADIENT_RADIAL_A:
				{
					float sc = (float)dot(
						Vector2f(i - ((float)width / 2.0f), j - ((float)height / 2.0f)).normalized(), 
						v.normalized());
					fin = col1 + ColorRGBAf(sc * dif.red, 
						sc * dif.green, sc * dif.blue, alpha ? sc * dif.alpha : 1.0f);
					break;
				}
			}

			// Normalize the result.
			if (fin.red < 0.0f)
				fin.red = 0.0f;
			else if (fin.red > 1.0f)
				fin.red = 1.0f;

			if (fin.green < 0.0f)
				fin.green = 0.0f;
			else if (fin.green > 1.0f)
				fin.green = 1.0f;

			if (fin.blue < 0.0f)
				fin.blue = 0.0f;
			else if (fin.blue > 1.0f)
				fin.blue = 1.0f;

			if (fin.alpha < 0.0f)
				fin.alpha = 0.0f;
			else if (fin.alpha > 1.0f)
				fin.alpha = 1.0f;

			img->pixels().data()[idx] = fin;	
	}

	return 0;
}

int linear_horizontal_gradient(Image *img, const unsigned int width, const unsigned int height,
	const ColorRGBAf &col1, const ColorRGBAf &col2, const Vector2f &start, const Vector2f &end, 
	const bool alpha)
{
	if(!img)
		return 1;

	if(!width || !height)
		return 2;

	if(img->init(width, height))
		return 3;

	scalar_t a = end.x - start.x;
	scalar_t b = end.y - start.y;

	scalar_t c1 = a*start.x + b*start.y;
	scalar_t c2 = a*end.x + b*end.y;

	for(unsigned int j = 0; j < height; j++)
		for(unsigned int i = 0; i < width; i++) {
			
			unsigned int idx = j * width + i;
			
			ColorRGBAf fin;
			scalar_t c = a*i+b*j;
			if(c <= c1){
				fin = col1;
			}else if(c >= c2){
				fin = col2;
			}else{
				scalar_t dc = c2-c1;
				fin = (col1*static_cast<float>(c2-c)+col2*static_cast<float>(c-c1))*(static_cast<float>(1.0f/dc)); //the first 2 casts are necessary because ColorRGBAf multiplication uses floats
				if(!alpha) fin.alpha = 1.0f;
			}

			// Normalize the result.
			if (fin.red < 0.0f)
				fin.red = 0.0f;
			else if (fin.red > 1.0f)
				fin.red = 1.0f;

			if (fin.green < 0.0f)
				fin.green = 0.0f;
			else if (fin.green > 1.0f)
				fin.green = 1.0f;

			if (fin.blue < 0.0f)
				fin.blue = 0.0f;
			else if (fin.blue > 1.0f)
				fin.blue = 1.0f;

			if (fin.alpha < 0.0f)
				fin.alpha = 0.0f;
			else if (fin.alpha > 1.0f)
				fin.alpha = 1.0f;

			img->pixels().data()[idx] = fin;	
	}

	return 0;
}


		} /* namespace Generator */
	} /* namespace Util */
} /* namespace TEXEngine */