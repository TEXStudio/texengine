#include "font.hpp"

namespace TEXEngine{
	namespace UI{

//Font::Font(const char* name, const int fontSize, const int fontStyle) : m_size(fontSize), m_style(fontStyle), m_name(name){
Font::Font(const ID id, const unsigned int gw, const unsigned int gh) : m_id(id), m_grid_width(gw), m_grid_height(gh), m_horizontal_shrinkage(0.0f), m_vertical_shrinkage(0.0f) {

}

Font::~Font(){

}


	}//namespace UI
}//namespace TEXEngine
