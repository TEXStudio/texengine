#ifndef TEXENGINE_CHECKBOX_HPP_INCLUDED
#define TEXENGINE_CHECKBOX_HPP_INCLUDED

#include "uiComponent.hpp"

namespace TEXEngine{
	namespace Core{
		class UIManager;
	}//forward declaration
	namespace UI{

class Checkbox : public TEXEngine::UI::UIComponent{
	friend class TEXEngine::Core::UIManager;

protected:
	void render() const;

	Checkbox(const ID);

	void system_behaviour(const unsigned long, const TEXEngine::Core::Message&);

	COMPONENT_TYPE get_type() const{
		return COMPONENT_TYPE_CHECKBOX;
	}

private:

	TEXEngine::Util::ColorRGBAf m_cross_color;

	float m_pixel_size;

	bool m_checked;

	void toggleChecked(){
		m_checked = !m_checked;
	}

public:

	~Checkbox();

	bool is_checked() const{
		return m_checked;	
	}

	void set_checked(const bool b){
		m_checked = b;
	}

	TEXEngine::Util::ColorRGBAf get_cross_color() const{
		return m_cross_color;
	}

	void set_cross_color(const TEXEngine::Util::ColorRGBAf& rgba){
		m_cross_color = rgba;
	}

	float get_pixel_size() const{
		return m_pixel_size;
	}

	void set_pixel_size(const float p){
		m_pixel_size = p;
	}

};

	}//namespace UI
}//namespace TEXEngine

#endif /* TEXENGINE_CHECKBOX_HPP_INCLUDED */