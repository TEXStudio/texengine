#include "..\log\logmanager.hpp"
#include "uiComponent.hpp"
#include "uiManager.hpp"
#include "container.hpp"

using namespace TEXEngine;
using namespace TEXEngine::Core;
using namespace TEXEngine::Util;

namespace TEXEngine{
	namespace UI{

unsigned long UIComponent::last_event_code = 0;

Message* UIComponent::last_message= NULL;

std::string* UIComponent::last_function = NULL;

UIComponent::UIComponent(const ID id) : m_x(-1.0f), m_y(-1.0f), m_width(0.0f), m_height(0.0f), m_border_thickness(0.01f),
	m_bgcolor(ColorRGBAf(0.0f, 0.0f, 0.0f, 1.0f)), m_border_color(ColorRGBAf(1.0f, 1.0f, 1.0f, 1.0f)), m_id(id), m_parent(0), m_handler(""), m_visible(true), m_render_border(false), m_enabled(true) {
}

float UIComponent::get_x() const{
	float x = m_x;
	Container* parent = UIManager::get().get_container(m_parent);
	if(parent){
		x+=parent->get_x();
	}
	return x;
}

float UIComponent::get_y() const{
	float y = m_y;
	Container* parent = UIManager::get().get_container(m_parent);
	if(parent){
		y+=parent->get_y();
	}
	return y;
}

float UIComponent::get_width() const{
	float w = m_width;
	Container* parent = UIManager::get().get_container(m_parent);
	if(parent){
		float x = get_x();
		float px = parent->get_x();
		float pw = parent->get_width();
		if(x + m_width > px + pw){
			w = px + pw - x;
		}
	}
	return (w < 0 ? 0 : w);
}

float UIComponent::get_height() const{
	float h = m_height;
	Container* parent = UIManager::get().get_container(m_parent);
	if(parent){
		float y = get_y();
		float py = parent->get_y();
		float ph = parent->get_height();
		if(y + m_height > py + ph){
			h = py + ph - y;
		}
	}
	return (h < 0 ? 0 : h);
}

bool UIComponent::is_visible() const{
	bool ret = m_visible;
	Container* parent = UIManager::get().get_container(m_parent);
	if(parent){
		ret = ret & parent->is_visible();
	}
	return ret;
}

int UIComponent::run_function(lua_State* state){
	int return_value = 0;

	if(state){
		if(last_message  && last_function){
			int	nargs = 2;

			lua_getglobal(state,"UI.__frame_on_event_functions");
			lua_getfield(state,-1,last_function->c_str());
			lua_getfield(state,-1,"_function");
			lua_getfield(state,-2,"_frame");
			lua_remove(state,-3);
			lua_remove(state,-3);
			
			
			if ( lua_isfunction(state,-2) )
			{
				lua_pushinteger(state,static_cast<lua_Integer>(last_event_code));

				for(unsigned int i = 0;  i < last_message->get_integral_data_size();  ++i){
					lua_pushinteger(state,static_cast<lua_Integer>(last_message->get_integral_data(i)));
					++nargs;
				}

				for(unsigned int i = 0;  i < last_message->get_real_data_size();  ++i){
					lua_pushnumber(state,static_cast<lua_Number>(last_message->get_real_data(i)));
					++nargs;
				}

				for(unsigned int i = 0;  i < last_message->get_pointers_size();  ++i){
					std::string* value = static_cast<std::string*>(last_message->get_pointer(i));

					if (value){
						lua_pushstring(state,value->c_str());
						++nargs;
					}
				}

				return_value = lua_pcall(state,nargs,0,0);
			}
		}
	}

	return return_value;
}

void UIComponent::on_event(LuaState& state , const unsigned long event_code ,  const TEXEngine::Core::Message& parameters){
	if (is_enabled() && m_handler != ""/* && is_visible()*/) {
		last_event_code = event_code;
		last_message = const_cast<Message*>(&parameters);
		last_function = &m_handler;

		if ( state.run_function(run_function) )
			LogManager::get().log_error(state.get_stack().c_str());
	}
}

void UIComponent::system_behaviour(const unsigned long, const TEXEngine::Core::Message&){
	if(!is_enabled()) return;

}

void UIComponent::render_border() const{
	ColorRGBAf color = m_border_color;
	float x = get_x();
	float y = get_y();
	float w = get_width();
	float h = get_height();
	if(m_border_thickness < w && m_border_thickness < h){
		UIManager::get().render_box(x, y, m_border_thickness, h, color);//left
		UIManager::get().render_box(x+w-m_border_thickness, y, m_border_thickness, h, color);//right
		//make sure the bottom and top borders don't overlap on the left and right ones because it will mess borders that have a colour with transparency (i.e. with alpha < 1.0)
		UIManager::get().render_box(x+m_border_thickness, y, w-2*m_border_thickness, m_border_thickness, color);//bottom
		UIManager::get().render_box(x+m_border_thickness, y+h-m_border_thickness, w-2*m_border_thickness, m_border_thickness, color);//top
	}
}

void UIComponent::set_tabindex(const unsigned int i) const{
	UIManager::get().add_tabindex(i, get_id());
}

UIComponent::~UIComponent() {
	if(m_parent){
		Container* c = UIManager::get().get_container(m_parent);
		if(c)
			c->detach(m_id);
	}
}

	}//namespace UI
}//namespace TEXEngine
