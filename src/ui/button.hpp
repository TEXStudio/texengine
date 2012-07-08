#ifndef TEXENGINE_BUTTON_HPP_INCLUDED
#define TEXENGINE_BUTTON_HPP_INCLUDED

#include "uiComponent.hpp"

namespace TEXEngine{
	namespace Core{
		class UIManager;
	}//forward declaration
	namespace UI{

class Button : public TEXEngine::UI::UIComponent{

	friend class TEXEngine::Core::UIManager;

protected:
	bool m_state;//true=>pressed

	void set_state(const bool s){
		m_state = s;
	}
	
	void render() const;

	void system_behaviour(const unsigned long, const TEXEngine::Core::Message&);

	Button(const ID);

	COMPONENT_TYPE get_type() const{
		return COMPONENT_TYPE_BUTTON;
	}

public:

	~Button();
	
	bool is_down() const{
		return m_state;
	}

};

	}//namespace UI
}//namespace TEXEngine

#endif /* TEXENGINE_BUTTON_HPP_INCLUDED */