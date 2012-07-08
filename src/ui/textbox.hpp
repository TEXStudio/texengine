#ifndef TEXENGINE_TEXTBOX_HPP_INCLUDED
#define TEXENGINE_TEXTBOX_HPP_INCLUDED

#include <string>
#include <cmath>
#include <vector>

#include "textComponent.hpp"
#include "../port/Timer.h"
#include "../message/TEXEngineInterface.hpp"

using TEXEngine::Port::Timer;

namespace TEXEngine{
	namespace Core{
		class UIManager;
	}//forward declaration
	namespace UI{

class Textbox : public TEXEngine::UI::TextComponent{

friend class TEXEngine::Core::UIManager;

private:

	static bool m_shift_down;
	static bool m_caps_down;

	mutable Timer m_timer;

	TEXEngine::Util::ColorRGBAf m_caret_color;
	std::vector<std::string>	m_lines;

	mutable double m_oldTime;
	double m_blinkDelay;
	unsigned int m_caretPosition; //this gives the (zero-based) location of the next input character
	mutable unsigned int caretRow;
	mutable unsigned int caretCol;

	float m_letterSpace;
	
	float m_letter_height;

	mutable bool m_showCaret;
	bool m_hasFocus;
	bool m_editable;
	bool m_multiline;
	bool m_wrap;

	unsigned int get_caret_position() const{
		return m_caretPosition;
	}

	void setCaretPosition(const unsigned int p){
		m_caretPosition = p;
	}

	void moveCaret(int delta){
		using namespace std;
		string t(get_text());
		m_caretPosition = max(static_cast<unsigned int>(0), min(t.length(),m_caretPosition+delta));
	}

	void renderCaret(const float paddingX, const float dx, const float letterSize, const unsigned int currentLine, const unsigned int xFactor) const;

	bool is_printable(const unsigned int e) const{
		return ((e >= TEXEngine::Core::KEYBOARD_KEY_ZERO_DOWN && e <= TEXEngine::Core::KEYBOARD_KEY_NINE_DOWN)
				|| (e >= TEXEngine::Core::KEYBOARD_KEY_A_DOWN && e <= TEXEngine::Core::KEYBOARD_KEY_Z_DOWN)
				|| e == TEXEngine::Core::KEYBOARD_KEY_SPACE_DOWN
				|| (e >= TEXEngine::Core::KEYBOARD_KEY_NUMPAD0_DOWN && e <= TEXEngine::Core::KEYBOARD_KEY_NUMPAD9_DOWN )
				|| (e >= TEXEngine::Core::KEYBOARD_KEY_MULTIPLY_DOWN && e <= TEXEngine::Core::KEYBOARD_KEY_DIVIDE_DOWN)
				|| (e >= TEXEngine::Core::KEYBOARD_KEY_SEMICOLON_DOWN && e <= TEXEngine::Core::KEYBOARD_KEY_TILDE_DOWN)
				|| (e >= TEXEngine::Core::KEYBOARD_KEY_LEFTBRACKET_DOWN && e <= TEXEngine::Core::KEYBOARD_KEY_DOUBLEQUOTES_DOWN)
			);
	}

	bool is_character(const unsigned int e) const{
		return e >= TEXEngine::Core::KEYBOARD_KEY_A_DOWN && e <= TEXEngine::Core::KEYBOARD_KEY_Z_DOWN;
	}

	void refactor_lines( const std::string& text );

protected:
	void render() const;
	
	Textbox(const ID id, const ID font, const char* t="");
	
	void system_behaviour(const unsigned long, const TEXEngine::Core::Message&);

	COMPONENT_TYPE get_type() const{
		return COMPONENT_TYPE_TEXTBOX;
	}

public:

	~Textbox();
	
	TEXEngine::Util::ColorRGBAf get_caret_color() const{
		return m_caret_color;
	}

	float get_letter_height() const{
		return m_letter_height;
	}

	void set_blink_delay(const double d){
		m_blinkDelay = d;
	}

	bool is_editable() const{
		return m_editable;
	}

	void set_editable(const bool b){
		m_editable = b;
	}

	void set_multiline(const bool b = true){
		m_multiline = b;
		refactor_lines(get_text_as_string());
	}

	void set_wrapping(const bool b = true){
		m_wrap = b;
	}

	void set_caret_color(const TEXEngine::Util::ColorRGBAf& rgba){
		m_caret_color = rgba;
	}

	void set_letter_height(const float h){
		m_letter_height = h;
	}

	void set_text(const char* text){
		set_text(std::string(text));
	}

	void set_text(const std::string& text){
		TextComponent::set_text(text);
		m_caretPosition = m_text.size();
		refactor_lines(text);
	}

	virtual void set_width(const float w){
		TextComponent::set_width(w);
		refactor_lines(get_text_as_string());
	}

	unsigned int visible_lines() const
	{
		return static_cast<unsigned int>((TextComponent::get_height()-m_letterSpace) / ( m_letter_height + m_letterSpace ));
	}
	unsigned int visible_characters() const
	{
		return static_cast<unsigned int>((TextComponent::get_width()-m_letterSpace) / ( m_letter_height + m_letterSpace ));
	}

};

	}//namespace UI
}//namespace TEXEngine

#endif /* TEXENGINE_TEXTBOX_HPP_INCLUDED */