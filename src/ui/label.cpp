#include <string>

#include "label.hpp"
#include "uiManager.hpp"
#include "container.hpp"
#include "font.hpp"

#include "../message/TEXEngineInterface.hpp"

using namespace std;

using namespace TEXEngine::Core;
using namespace TEXEngine::Util;

namespace TEXEngine{
	namespace UI{

Label::Label(const ID id, const ID font, const char* text) : TextComponent(id, font, text){

}

void Label::system_behaviour(const unsigned long e, const TEXEngine::Core::Message&){
	if(!is_enabled()) return;

	if(e == COMPONENT_ON_FOCUS_GAINED){
		m_render_border = true;
	}else if(e == COMPONENT_ON_FOCUS_LOST){
		m_render_border = false;
	}	
}

void Label::render() const{
	if(!is_visible())
		return;

	ColorRGBAf color = get_background_color();
	float x = get_x();
	float y = get_y();
	float w = get_width();
	float h = get_height();

	UIManager::get().render_box(x, y, w, h, color);
	
	//Draw text
	string text(get_text());
	unsigned int size = text.length();
	//float letterSize = 0.05f;
	float letterSize = min(w,h);
	if(h < letterSize)
		letterSize = h/2.0f;
	//float space = 0.01f;
	float space = 0.0f;
	float dx = letterSize + space;

	//float paddingX = (w-dx*size)/2.0f;
	float paddingX = compute_padding_x(letterSize);
	if(paddingX < 0)//if there isn't enough space in the label to contain all the text, use an arbitrary amount
		paddingX = space;
	//Draw text
	for(unsigned int i = 0; i < size; ++i){
		float px = paddingX + x+dx*i;
		//if(px+letterSize+paddingX>=x+w)
		if(px+letterSize>x+w)
			break;

			//px = x+w-letterSize;

		//UIManager::get().render_box(px, y+(h-letterSize)/2.0f, letterSize, letterSize, get_background_color());
		render_character(text.at(i), px, y+(h-letterSize)/2.0f, letterSize, letterSize);
	}

	//Draw outline if needed
	if(m_render_border){
		render_border();
	}
}

Label::~Label(){
}

	}//namespace UI
}//namespace TEXEngine
