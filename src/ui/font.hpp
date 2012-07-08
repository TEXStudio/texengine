#ifndef TEXENGINE_FONT_HPP_INCLUDED
#define TEXENGINE_FONT_HPP_INCLUDED

#include <string>

#include "uiUtility.hpp"

namespace TEXEngine{
	namespace Core{
		class UIManager;
	}//forward declaration

	namespace UI{

class Font{
friend class TEXEngine::Core::UIManager;

private:
	ID m_id;

	unsigned int m_grid_width;
	unsigned int m_grid_height;

	float m_horizontal_shrinkage;
	float m_vertical_shrinkage;
	
protected:
	
	//this is a font that is based on a ppm image of 16 characters per 16 characters, if somehow we want to change that (which is very unlikely), there is the possibility
	Font(const ID id, const unsigned int gw = 16, const unsigned int gh = 16);

public:

	~Font();

	ID get_id() const{
		return m_id;
	}

	unsigned int get_grid_width() const{
		return m_grid_width;
	}
	
	unsigned int get_grid_height() const{
		return m_grid_height;
	}
	
	//the "shrinkage" is an amount by which the uv coordinates are reduced
	//if the following represents the quad that will have the font texture
	// u4____u3
	//  |    |
	//  |----|
	//  u1   u2
	//then the horizontal shrinkage is added to u1.s and substracted from u2.s   (and added to u4.s and subtracted from u3.s)
	//and the vertical shrinkage is added to u1.t and substracted from u4.t      (and added to u2.t and substracted from u3.t)
	//therefore these numbers are intended to be positive
	//
	//this is needed because the image that we generate for the font may have too much space between the letters
	//so this can be used to reduce the space
	float get_horizontal_shrinkage() const{
		return m_horizontal_shrinkage;
	}

	float get_vertical_shrinkage() const{
		return m_vertical_shrinkage;
	}

	void set_horizontal_shrinkage(const float h) {
		m_horizontal_shrinkage = h;
	}

	void set_vertical_shrinkage(const float v) {
		m_vertical_shrinkage = v;
	}

};

	}//namespace UI
}//namespace TEXEngine

#endif /* TEXENGINE_FONT_HPP_INCLUDED */