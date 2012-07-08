#include <string>
#include <cmath>

#include "slider.hpp"
#include "uiManager.hpp"
#include "container.hpp"
#include "../message/TEXEngineInterface.hpp"

using namespace std;
using namespace TEXEngine::Core;
using namespace TEXEngine::Util;

namespace TEXEngine{
	namespace UI{

Slider::Slider(const ID id, const int steps, const int min, const int max) : UIComponent(id), m_marker_color(ColorRGBAf(0.0f, 0.0f, 0.0f, 1.0f)), 
	m_value(min), m_min(min), m_max(max), m_steps(steps), m_marker_width(0.05f), m_marker_height_padding(0.01f), m_marker_width_padding(0.01f){
	if(min > max){
		m_min = max;
		m_max = min;
		m_value = m_min;
	}
	
	if(steps > m_max || steps < m_min){
		m_steps = (m_max-m_min)/2;
	}
}
	
void Slider::set_value(const int v){
	int old = m_value;
	m_value = v;

	//make sure m_value is still such that m_min <= m_value <= m_max
	m_value = max(m_min, min(m_value, m_max));
	if( m_handler != "" ){
		Message m(5);
		m.add_integral_data(0, 0);
		m.add_integral_data(1, 0);
		m.add_integral_data(2, get_id());
		m.add_integral_data(3, old);
		m.add_integral_data(4, m_value);
		on_event(UIManager::get().get_state(),COMPONENT_ON_VALUE_CHANGED, m);
	}
}

void Slider::system_behaviour(const unsigned long e, const TEXEngine::Core::Message& params){
	static bool leftDown = false;
	static float SLIDER_EPSILON_TEST = 0.0015f;
	
	if(!is_enabled()) return;

	if(e == COMPONENT_ON_FOCUS_GAINED){
		m_render_border = true;
	}else if(e == COMPONENT_ON_FOCUS_LOST){
		leftDown = false;
		m_render_border = false;
	}else if(e == MOUSE_BUTTON_LEFT_UP){
		leftDown = false;
	}else if(e == MOUSE_BUTTON_LEFT_DOWN || (e == MOUSE_POSITION_CHANGED && leftDown)){
		if(e == MOUSE_BUTTON_LEFT_DOWN)
			leftDown = true;
		double mx = params.get_real_data(0);
		float x = get_x();
		float w = get_width();
		float paddingX = m_marker_width_padding;
		if(mx < x+paddingX-SLIDER_EPSILON_TEST /* hax0r :( */ || mx >= x+paddingX+w-m_marker_width){
			return;//outside slidable area
		}
		//the general idea for this computation is as follows:
		//where the mouse is located is where the leftmost part of the marker should be
		double minn = x+paddingX;
		double maxx = x+paddingX+w-m_marker_width;
		//slidable area is within [minn, maxx]
		double f = (mx-minn)/(maxx-minn);
		double d = f*(m_max-m_min)+m_min;

		//int k = (int)floor(d);
		int k = (int)floor(d+0.5);
		//int k = static_cast<int>(d);
		set_value(k);

		/*
		#ifdef _WIN32
		GUITHREADINFO ti;
		ti.cbSize = sizeof(GUITHREADINFO);
		GetGUIThreadInfo(0, &ti);
		char buff[60];
		//sprintf(buff, "mx=%.2f, x=%.2f, w=%.2f, v=%d, d=%.2f f=%.2f\0", mx, x, w, m_value, d, f);
		sprintf(buff, "mx=%.2f, min=%.2f, max=%.2f, f=%.2f, d=%.2f,k=%d", mx, minn, maxx, f, d, k);
		SetWindowText(ti.hwndActive, buff);
		#endif
		*/
	}else if(e == COMPONENT_ACTION_LEFT){
		set_value(get_value()-1);
	}else if(e == COMPONENT_ACTION_RIGHT){
		set_value(get_value()+1);
	}
}

void Slider::render() const{
	if(!is_visible())
		return;

	ColorRGBAf color = get_background_color();
	float x = get_x();
	float y = get_y();
	float w = get_width();
	float h = get_height();

	UIManager::get().render_box(x, y, w, h, color);

	float myMarkerSize = m_marker_width;
	if(h < myMarkerSize )
		myMarkerSize = h/2.0f;
	if(w < myMarkerSize)
		myMarkerSize = w/2.0f;

	color = m_marker_color;
	float paddingX = m_marker_width_padding;
	float width = w-2*paddingX -myMarkerSize ;
	if(width > 0){
		float fract = (float)(m_value-m_min)/(m_max-m_min);
		//UIManager::get().render_box(x+paddingX+fract*width, y+m_marker_height_padding, myMarkerSize, h-2*m_marker_height_padding);
		UIManager::get().render_box(x+paddingX+fract*width, y+m_marker_height_padding, myMarkerSize, h-2*m_marker_height_padding, color);
	}

	//Draw outline if needed
	if(m_render_border){
		render_border();
	}
}

Slider::~Slider(){
}

	}//namespace UI
}//namespace TEXEngine
