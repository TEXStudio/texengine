#include "button.hpp"
#include "uiManager.hpp"
#include "container.hpp"

#include "../message/TEXEngineInterface.hpp"


using namespace TEXEngine::Core;
using namespace TEXEngine::Util;

namespace TEXEngine{
	namespace UI{


Button::Button(const ID id) : UIComponent(id){

}

void Button::system_behaviour(const unsigned long e, const TEXEngine::Core::Message&){
	if(!is_enabled()) return;

	if(e == COMPONENT_ON_FOCUS_GAINED){
		m_render_border = true;
	}else if(e == COMPONENT_ON_FOCUS_LOST){
		m_render_border = false;
	}
}

void Button::render() const{
	if(!is_visible())
		return;

	ColorRGBAf color = get_background_color();
	float x = get_x();
	float y = get_y();
	float w = get_width();
	float h = get_height();

	UIManager::get().render_box(x, y, w, h, color);

	//Draw outline if needed
	if(m_render_border){
		render_border();
	}
}

Button::~Button(){
}

	}//namespace UI
}//namespace TEXEngine