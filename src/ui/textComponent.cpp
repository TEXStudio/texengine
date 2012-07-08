#include "textComponent.hpp"
#include "font.hpp"
#include "uiManager.hpp"
#include "../math/vector.hpp"

using namespace TEXEngine;
using namespace TEXEngine::Util;
using namespace TEXEngine::Core;
using namespace TEXEngine::Math;

namespace TEXEngine{
	namespace UI{


TextComponent::TextComponent(const ID id, const ID font, const char* text) : UIComponent(id), m_font(font), m_text(text), m_horizontal_align(TEXENG_ALIGN_CENTER) {

}

TextComponent::~TextComponent(){

}

void TextComponent::render_character(const char c, const float x, const float y, const float w, const float h) const{
	Font* font = UIManager::get().get_font(get_font_id());
	if(font){
		unsigned int font_gridwidth = font->get_grid_width();
		unsigned int font_gridheight = font->get_grid_height();

		//work out texel coordinates
		unsigned int grid_row = c/font_gridheight;
		unsigned int grid_col = c%font_gridwidth;
		float texel_width = 1.0f/font_gridwidth;
		float texel_height = 1.0f/font_gridheight;
		
		//float xStuff = 0.015f;
		float yStuff = font->get_vertical_shrinkage();
		float xStuff = font->get_horizontal_shrinkage();
		//float xStuff = 0.0f;
		Vector2f uv1(min(1.0f, grid_col*texel_width+xStuff), max(0.0f,grid_row*texel_height-yStuff));
		Vector2f uv2(max(0.0f, (grid_col+1)*texel_width-xStuff), max(0.0f,grid_row*texel_height-yStuff));
		Vector2f uv3(max(0.0f, (grid_col+1)*texel_width-xStuff), min(1.0f, (grid_row+1)*texel_height+yStuff));
		Vector2f uv4(min(1.0f, grid_col*texel_width+xStuff), min(1.0f, (grid_row+1)*texel_height+yStuff));

		int texid = font->get_id();
		UIManager::get().render_box(x, y, w*1.0f/UIManager::get().aspect_modifier(), h, ColorRGBAf(), texid, uv4, uv3, uv2, uv1);//TM: tests indicated that this is this use of the aspect_modifier is the way to make the text look good in all situations
	}
}

float TextComponent::compute_padding_x(const float lh) const{
	float letterSize = lh;
	float h = get_height();
	float w = get_width();
	if(h < letterSize)
		letterSize = h/2.0f;
	float dx = letterSize;
	std::string text(get_text());
	unsigned int size = text.length();

	switch(m_horizontal_align){
		case TEXENG_ALIGN_LEFT: return 0.0f;

		default:
		case TEXENG_ALIGN_CENTER:
			return (w-dx*size)*0.5f;

		case TEXENG_ALIGN_RIGHT: return (w-dx*(size+1));
		
	}

}

	}//namespace UI
}//namespace TEXEngine
