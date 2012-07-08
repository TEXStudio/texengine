#include <cstdio>
#include "ppm.hpp"

using TEXEngine::Util::ColorRGBAf;

namespace TEXEngine {
	namespace Util {
		namespace IO {
			namespace Import {

/* 
	NOTES:
	In this implementation I am using c-style I/O.
	This is due to our latest discovery that the PSP
	does not implement a large portion of STL including
	streams. Still undecided if it is worth implementing
	the missing STL utils, porting the rest of the code
	or to drop the psp version all together.
*/

/*
	FORMAT: PPM RAW
	TYPE: BINARY

	HEADER:
	1. magic number: P6
	2. Whitespace (blanks, TABs, CRs, LFs).
	3. A width per pixel component, formatted as ASCII character in decimal.
	4. Whitespace.
	5. A height, again in ASCII decimal.
	6. Whitespace.
	7. The maximum color value (Maxval), again in ASCII decimal.
	8. Must be less than 65536 and more than zero.
	9. A single whitespace character (usually a newline).

	Pixel data then follows.

	Comments can be placed anywhere starting with the character '#'.
*/

int ppm_raw(const char *filename, Image *img)
{
	if(!filename)
		return 1;

	if(!img)
		return 2;
	
	FILE *fp = fopen(filename, "rb");

	if (!fp)
		return 3;

	char magic_number[3] = {'\0', '\0', '\0'} ;
	fgets(magic_number,sizeof(magic_number),fp);

	// Check for correct version.
	if(!(  magic_number[0] == 'P' 
		&& magic_number[1] == '6'
		&& magic_number[2] == '\0')) {
		fclose(fp);
		return 4;
	}
	
	unsigned int width = 0, height = 0, bpc = 0;
	fscanf(fp, "%i %i %i ", &width, &height, &bpc);
	
	// Check for correct pixel component size.
	if(bpc != 255) {
		fclose(fp);
		return 4;
	}

	// check for rational dimensions
	if(!width || !height) {
		fclose(fp);
		return 5;
	}

	//init image
	if(img->init(width, height)) {
		fclose(fp);
		return 6;
	}

	Buffer<ColorRGBAf> *p_imgp = &(img->pixels_w());

	// Read pixels.
	for (unsigned int i = 0; i < width * height; i++) {
			p_imgp->data()[i].red	= (float)fgetc(fp) / 255.0f;
			p_imgp->data()[i].green = (float)fgetc(fp) / 255.0f;
			p_imgp->data()[i].blue	= (float)fgetc(fp) / 255.0f;
			p_imgp->data()[i].alpha = 1.0f;

			// Check for errors
			if(ferror(fp) || feof(fp)) {
				// The pixels that were read so far are
				// being kept in the image buffer.
				fclose(fp);
				return 5;
			}
		}

	fclose(fp);

	return 0;
}

			} /* namespace Import */

			namespace Export {

int ppm_raw(const char *filename, Image *img)
{
	// check for rational dimensions
	if(!filename)
		return 1;

	if(!img)
		return 2;

	if(!img->width() || !img->height())
		return 3;
	
	FILE *fp = fopen(filename, "wb");

	if (!fp)
		return 4;

	fprintf(fp, "P6\n%d %d\n255\n", img->width(), img->height());

	Buffer<ColorRGBAf> *p_imgp = &(img->pixels_w());

	// Write pixels.
	for (unsigned int i = 0; i < img->width() * img->height(); i++) {
			fputc((int)(p_imgp->data()[i].red	* 255.0f), fp);
			fputc((int)(p_imgp->data()[i].green * 255.0f), fp);
			fputc((int)(p_imgp->data()[i].blue	* 255.0f), fp);

			// Check for errors
			if(ferror(fp) || feof(fp)) {
				// The pixels that were read so far are
				// being kept in the image buffer.
				fclose(fp);
				return 5;
			}
		}

	fclose(fp);

	return 0;
}

			} /* namespace Export */

		} /* namespace IO */
	} /* namespace Util */
} /* namespace TEXEngine */