#include "checkbox.hpp"
#include "uiManager.hpp"
#include "container.hpp"
#include "uiUtility.hpp"
#include "../message/TEXEngineInterface.hpp"


using namespace TEXEngine::Core;
using namespace TEXEngine::Util;

namespace TEXEngine{
	namespace UI{

Checkbox::Checkbox(const ID id) : UIComponent(id), m_cross_color(0.0f, 0.0f, 0.0f, 1.0f), m_pixel_size(0.01f), m_checked(false){
	
}

void Checkbox::system_behaviour(const unsigned long e, const TEXEngine::Core::Message&){
	if(!is_enabled()) return;

	switch(e){
		case COMPONENT_ACTION_PERFORMED:
			toggleChecked();
			break;
		case COMPONENT_ON_FOCUS_GAINED:
			m_render_border = true;
			break;
		case COMPONENT_ON_FOCUS_LOST:
			m_render_border = false;
			break;
	}
}

void Checkbox::render() const{
	if(!is_visible())
		return;

	ColorRGBAf color = get_background_color();
	bool doCross = is_checked();
	float x = get_x();
	float y = get_y();
	float w = get_width();
	float h = get_height();
	
	UIManager::get().render_box(x, y, w, h, color);

	if(doCross){
		float heightFactor = 1.0f, widthFactor = 1.0f;
		if(h > w){
			heightFactor = h/w;
		}else if(w > h){
			widthFactor = w/h;
		}
		float hShizzle = h/(heightFactor*m_pixel_size);
		unsigned int iter = static_cast<unsigned int>(hShizzle); //can do it with w or h it doesn't matter because the intention is to have the cross "square shaped" anyway
		for(unsigned int i = 0; i < iter; ++i){
			for(unsigned int j = 0; j < iter; ++j){
				if(i==j || i+j == iter-1)
					UIManager::get().render_box(x+i*widthFactor*m_pixel_size, y+j*heightFactor*m_pixel_size, widthFactor*m_pixel_size, heightFactor*m_pixel_size, m_cross_color); 
			}
		}
	}

	//Draw outline if needed
	if(m_render_border){
		render_border();
	}
}

Checkbox::~Checkbox(){
}

	}//namespace UI
}//namespace TEXEngine