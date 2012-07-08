#ifndef TEXENGINE_TEXTCOMPONENT_HPP_INCLUDED
#define TEXENGINE_TEXTCOMPONENT_HPP_INCLUDED

#include <string>

#include "uiComponent.hpp"

#include "../util/color.hpp"

namespace TEXEngine{
	namespace UI{
	
class TextComponent : public TEXEngine::UI::UIComponent{
protected:
	std::string m_text;

	ID m_font;

	TextComponent(const ID id, const ID font, const char* text = "");

	//this function works out the 4 uv vectors that must be provided to the rendermanager
	void render_character(const char c, const float x, const float y, const float w, const float h) const;

public:

	enum TEXENG_TEXT_ALIGN{
		TEXENG_ALIGN_LEFT = 0,
		TEXENG_ALIGN_CENTER = 1,
		TEXENG_ALIGN_RIGHT = 2,
	};

	virtual ~TextComponent();

	virtual const char* get_text() const{
		return m_text.c_str();
	}

	virtual std::string get_text_as_string() const{
		return m_text;
	}

	virtual void set_text(const char* text){
		set_text(std::string(text));
	}

	virtual void set_text( const std::string& text ){
		m_text = text;
	}

	void set_font_id(const ID id){
		m_font = id;
	}

	ID get_font_id() const{
		return m_font;
	}

	void set_horizontal_align(const TEXENG_TEXT_ALIGN a){
		m_horizontal_align = a;
	}

	TEXENG_TEXT_ALIGN get_horizontal_align() const{
		return m_horizontal_align;
	}

protected:

	TEXENG_TEXT_ALIGN m_horizontal_align;

	float compute_padding_x(const float lh) const;

};

	}//namespace UI
}//namespace TEXEngine

#endif /* TEXENGINE_TEXTCOMPONENT_HPP_INCLUDED */