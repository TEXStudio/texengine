#ifndef TEXENGINE_SLIDER_HPP_INCLUDED
#define TEXENGINE_SLIDER_HPP_INCLUDED

#include "uiComponent.hpp"


namespace TEXEngine{
	namespace Core{
		class UIManager;
	}//forward declaration
	namespace UI{

class Slider : public TEXEngine::UI::UIComponent{
friend class TEXEngine::Core::UIManager;

private:

	TEXEngine::Util::ColorRGBAf m_marker_color;

	int m_value;
	int m_min;
	int m_max;
	int m_steps;


	float m_marker_width; //this is the width of the marker
	float m_marker_height_padding; //the marker's height is gonna be the same as the slider's, this variable defines internal vertical padding to reduce the height of the marker
	float m_marker_width_padding;

protected:
	void render() const;
	//steps define every "how many units of value" there should be a visual indicator, 
	//for instance if we want a slider with values ranging from 0 to 100 and use a step of 50
	//this is what we'll get:  |     |     |
	//                         0     50    100
	//
	//with a step of 10:       |||||||||||
	//                         012345678910
	//
	//note that the value labels are not part of this component, they're just on the above schemes
	//for clarity
	Slider(const ID, const int steps = 50, const int min = 0, const int max = 100);

	void system_behaviour(const unsigned long, const TEXEngine::Core::Message&);

	COMPONENT_TYPE get_type() const{
		return COMPONENT_TYPE_SLIDER;
	}


public:

	~Slider();

	int get_value() const{
		return m_value;
	}
	
	void set_value(const int v);

	void set_marker_color(const TEXEngine::Util::ColorRGBAf& rgba){
		m_marker_color = rgba;
	}

	void set_marker_width(const float w){
		m_marker_width = w;
	}

	void set_marker_height_padding(const float h){
		m_marker_height_padding = h;
	}

	void set_marker_width_padding(const float w){
		m_marker_width_padding = w;
	}
	
};

	}//namespace UI
}//namespace TEXEngine

#endif /* TEXENGINE_SLIDER_HPP_INCLUDED */