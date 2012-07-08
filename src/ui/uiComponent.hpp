#ifndef TEXENGINE_UICOMPONENT_HPP_INCLUDED
#define TEXENGINE_UICOMPONENT_HPP_INCLUDED

#include <string>
#include "script/luaState.hpp"
#include "uiUtility.hpp"
#include "../message/message.hpp"
#include "../util/color.hpp"

namespace TEXEngine{
	namespace Core{
		class UIManager;
	};//Forward declaration
	namespace UI{

class UIComponent{

	friend class Container;
	friend class Core::UIManager;

private:
	
	//	NOT FUCKING THREAD SAFE
	static unsigned long last_event_code;
	static TEXEngine::Core::Message* last_message;
	static std::string* last_function;

protected:
	//used internally for performances
	enum COMPONENT_TYPE{
		COMPONENT_TYPE_BUTTON, COMPONENT_TYPE_CHECKBOX, COMPONENT_TYPE_CONTAINER, COMPONENT_TYPE_IMAGE, COMPONENT_TYPE_LABEL, COMPONENT_TYPE_SLIDER, COMPONENT_TYPE_TEXTBOX
	};


	//x and y are relative to the parent container
	//therefore negative values for x and y, unless this component is a container, 
	//will position the component out of scope. It can still be visible if its 
	//width and height are high enough though
	float m_x, m_y, m_width, m_height;
	
	float m_border_thickness;

	TEXEngine::Util::ColorRGBAf m_bgcolor;
	TEXEngine::Util::ColorRGBAf m_border_color;

	ID m_id, m_parent;

	std::string m_handler;

	bool m_visible;

	bool m_render_border;

	bool m_enabled;
		
	virtual void render() const = 0;	

	void render_border() const;

	UIComponent(const ID id);

	static int run_function(lua_State* state);

	//runs the handler for this function
	void on_event(LuaState& state, const unsigned long event_code,  const TEXEngine::Core::Message& parameters);

	//called by uimanager before on_event to do perform component-specific behaviour
	virtual void system_behaviour(const unsigned long, const TEXEngine::Core::Message& m = TEXEngine::Core::Message());

	//little hack to avoid having to use a dynamic_cast somewhere in UIManager::on_event
	bool is_textbox() const{
		return get_type() == COMPONENT_TYPE_TEXTBOX;
	}

	void set_parent(const ID p){
		m_parent = p;
	}

	virtual COMPONENT_TYPE get_type() const = 0;

public:

	virtual ~UIComponent();

	ID get_id() const{
		return m_id;
	}

	ID get_parent() const{
		return m_parent;
	}
	
	float get_x() const;

	float get_y() const;
	
	float get_width() const;

	float get_height() const;

	TEXEngine::Util::ColorRGBAf get_background_color() const{
		return m_bgcolor;
	}
	
	bool is_visible() const;

	bool is_enabled() const{
		return m_enabled;
	}

	void set_x(const float x){
		m_x = x;
	}

	void set_y(const float y){
		m_y = y;
	}

	virtual void set_width(const float w){
		if(w >= 0)
			m_width = w;
	}

	virtual void set_height(const float h){
		if(h >= 0)
			m_height = h;
	}

	void set_background(const TEXEngine::Util::ColorRGBAf& rgba){
		m_bgcolor = rgba;
	}

	void set_border_color(const TEXEngine::Util::ColorRGBAf& rgba){
		m_border_color = rgba;
	}

	void set_border_thickness(const float thickness){
		if(thickness >= 0)
			m_border_thickness = thickness;
	}
	
	void set_visible(const bool vis){
		m_visible = vis;
	}
	
	void set_render_border(const bool r){
		m_render_border = r;
	}

	void set_enabled(const bool e){
		m_enabled = e;
	}

	//as funny as it may sound this function doesn't modify the state of this object, hence it can be const. 
	//supply 0 to unbind this component, making it unavailable to the ui navigation system (apart from the mouse)
	void set_tabindex(const unsigned int) const;
			
	void bind(std::string& name){
		m_handler = name;
	}

};

	}//namespace UI
}//namespace TEXEngine

#endif /* TEXENGINE_UICOMPONENT_HPP_INCLUDED */