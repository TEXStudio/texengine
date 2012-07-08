#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <iostream>

#include "textbox.hpp"
#include "uiManager.hpp"
#include "container.hpp"
#include "../log/logmanager.hpp"


using namespace std;
using namespace TEXEngine::Core;
using namespace TEXEngine::Util;

namespace TEXEngine{
	namespace UI{

bool Textbox::m_shift_down = false;
bool Textbox::m_caps_down = false;

Textbox::Textbox(const ID id, const ID font, const char* text) : TextComponent(id, font, text), m_caret_color(1.0f, 1.0f, 1.0f, 1.0f), 
	m_oldTime(0.0), m_blinkDelay(250), m_caretPosition(0), m_letterSpace(0.0f), //m_letterSpace(0.01f),
	m_letter_height(0.05f), caretRow(0) , caretCol(0) ,
	m_showCaret(false), m_hasFocus(false), m_editable(true), m_multiline(false), m_wrap(false){
	string p(text);
	m_caretPosition = p.size();
	m_timer.start();
}

void Textbox::system_behaviour(const unsigned long e, const TEXEngine::Core::Message&){

	if(!is_enabled()) return;

	if(e == COMPONENT_ON_FOCUS_GAINED){
		m_hasFocus = true;
		m_render_border = true;
	}else if(e == COMPONENT_ON_FOCUS_LOST){
		m_hasFocus = false;
		m_render_border = false;
	}else if(is_editable() && (e >= KEYBOARD_KEY_CANCEL_DOWN && e <= KEYBOARD_KEY_DOUBLEQUOTES_UP/*KEYBOARD_KEY_OEMCLEAR_DOWN*/)){
		unsigned int caret = get_caret_position();
		string currentText(get_text_as_string());
		string lastText(currentText);

		unsigned int nCharactersPerLine = (unsigned int)floor(get_width()/(0.05f+m_letterSpace));

		switch(e){
			case KEYBOARD_KEY_BACKSPACE_DOWN:{
				if(caret > 0){
					currentText.erase(caret-1,1);
					TextComponent::set_text(currentText);
					refactor_lines(currentText);
					moveCaret(-1);
				}
			break;
			}//KEYBOARD_KEY_BACKSPACE_DOWN
			case KEYBOARD_KEY_DELETE_DOWN:{
				if(caret < currentText.size()){
					currentText.erase(caret,1);
					TextComponent::set_text(currentText);
					refactor_lines(currentText);
				}
				break;
			}//KEYBOARD_KEY_DELETE_DOWN
			case KEYBOARD_KEY_HOME_DOWN:{
				setCaretPosition(0);
				break;
			}//KEYBOARD_KEY_HOME_DOWN
			case KEYBOARD_KEY_END_DOWN:{
				setCaretPosition(currentText.size());
				break;
			}//KEYBOARD_KEY_END_DOWN
			case KEYBOARD_KEY_LEFTARROW_DOWN:{
				if(caret <= 1)
					setCaretPosition(0);
				else{
					long long v = caret-1;
					if(v < 0)
						setCaretPosition(0);
					else
						setCaretPosition(static_cast<unsigned int>(v));
					//setCaretPosition(max(0, caret-1));
				}
					
				break;
			}
			case KEYBOARD_KEY_DOWNARROW_DOWN:{
				if(m_multiline){
					setCaretPosition(min(currentText.size(), caret+nCharactersPerLine));
				}else{
					if(caret <= 1)
						setCaretPosition(0);
					else{
						long long v = caret-1;
						if(v < 0)
							setCaretPosition(0);
						else
							setCaretPosition(static_cast<unsigned int>(v));
					}
				}
				break;
			} //KEYBOARD_KEY_LEFTARROW_DOWN, KEYBOARD_KEY_DOWNARROW_DOWN
			case KEYBOARD_KEY_RIGHTARROW_DOWN:{
				setCaretPosition(min(currentText.size(), caret+1));
				break;
			}
			case KEYBOARD_KEY_UPARROW_DOWN:{
				if(m_multiline){

					long long v = caret-nCharactersPerLine;
					if(v < 0)
						setCaretPosition(0);
					else
						setCaretPosition(static_cast<unsigned int>(v));
				}else{
					setCaretPosition(min(currentText.size(), caret+1));
				}
				break;
			} //KEYBOARD_KEY_RIGHTARROW_DOWN, KEYBOARD_KEY_UPARROW_DOWN
			case KEYBOARD_KEY_SHIFT_DOWN:
				m_shift_down = true;
				break;
			case KEYBOARD_KEY_SHIFT_UP:
				m_shift_down = false;
				break;
			case KEYBOARD_KEY_CAPSLOCK_DOWN:
				m_caps_down = true;
				break;			
			case KEYBOARD_KEY_CAPSLOCK_UP:
				m_caps_down = false;
				break;
			default:{

				bool capital = m_shift_down ^ m_caps_down;
				char c = '\0';

				if(is_printable(e)){
					
					if ( is_character(e)  )
						c = (capital ? 'A' : 'a') + (char)(e-KEYBOARD_KEY_A_DOWN);
					else if ( !m_shift_down  &&   e >= KEYBOARD_KEY_ZERO_DOWN  &&  e<= KEYBOARD_KEY_NINE_DOWN )
						c = '0' + (char)(e - KEYBOARD_KEY_ZERO_DOWN);
					else if ( m_shift_down  &&   e >= KEYBOARD_KEY_ZERO_DOWN  &&  e<= KEYBOARD_KEY_NINE_DOWN )
					{
						if ( e == KEYBOARD_KEY_ZERO_DOWN )
							c = ')';
						else if ( e == KEYBOARD_KEY_ONE_DOWN )
							c = '!';
						else if ( e == KEYBOARD_KEY_TWO_DOWN )
							c = '@';
						else if ( e == KEYBOARD_KEY_THREE_DOWN )
							c = '#';
						else if ( e == KEYBOARD_KEY_FOUR_DOWN )
							c = '$';
						else if ( e == KEYBOARD_KEY_FIVE_DOWN )
							c = '%';
						else if ( e == KEYBOARD_KEY_SIX_DOWN )
							c = '^';
						else if ( e == KEYBOARD_KEY_SEVEN_DOWN )
							c = '&';
						else if ( e == KEYBOARD_KEY_EIGHT_DOWN )
							c = '*';
						else if ( e == KEYBOARD_KEY_NINE_DOWN )
							c = '(';
					}
					else if ( e >= KEYBOARD_KEY_SEMICOLON_DOWN && e <= KEYBOARD_KEY_TILDE_DOWN )
					{
						if ( e == KEYBOARD_KEY_SEMICOLON_DOWN )
							c = ':' + ( m_shift_down ? (char)0 : (char)1 );
						else if ( e == KEYBOARD_KEY_PLUS_DOWN )
							c = '+' + ( m_shift_down ? (char)0 : (char)18 );
						else if ( e == KEYBOARD_KEY_COMMA_DOWN )
							c = ',' + ( !m_shift_down ?  (char)0 : (char)16 );
						else if ( e == KEYBOARD_KEY_MINUS_DOWN )
							c = '-' + ( !m_shift_down ?  (char)0 : (char)50 );
						else if ( e == KEYBOARD_KEY_DOT_DOWN ) 
							c = '.' + ( !m_shift_down ?  (char)0 : (char)16 );
						else if ( e == KEYBOARD_KEY_FORWARDSLASH_DOWN )
							c = '/' + ( !m_shift_down ?  (char)0 : (char)16 );
						else if ( e == KEYBOARD_KEY_TILDE_DOWN )
							c = '`' + ( m_shift_down ?  (char)0 : (char)30 );
					}
					else if ( e >= KEYBOARD_KEY_LEFTBRACKET_DOWN && e <= KEYBOARD_KEY_DOUBLEQUOTES_DOWN ) 
					{
						if ( e == KEYBOARD_KEY_LEFTBRACKET_DOWN )
							c = '[' + ( !m_shift_down ?  (char)0 : (char)32 );
						else if ( e == KEYBOARD_KEY_BACKWARDSSLASH_DOWN ) 
							c = '\\' + ( !m_shift_down ?  (char)0 : (char)32 );
						else if ( e == KEYBOARD_KEY_RIGHTBRACKET_DOWN )
							c = ']' + ( !m_shift_down ?  (char)0 : (char)32 );
						else if ( e == KEYBOARD_KEY_DOUBLEQUOTES_DOWN ) 
							c = '"' + ( m_shift_down ?  (char)0 : (char)5 );

					}
					else if ( e >= KEYBOARD_KEY_MULTIPLY_DOWN && e <= KEYBOARD_KEY_DIVIDE_DOWN )
					{
						if ( e == KEYBOARD_KEY_MULTIPLY_DOWN )
							c = '*';
						else if ( e == KEYBOARD_KEY_ADD_DOWN )
							c = '+';
						else if ( e == KEYBOARD_KEY_SUBTRACT_DOWN )
							c = '-';
						else if ( e == KEYBOARD_KEY_DECIMAL_DOWN )
							c = '.';
						else if ( e == KEYBOARD_KEY_DIVIDE_DOWN )
							c = '/';
					}
					else if ( c >= KEYBOARD_KEY_NUMPAD0_DOWN  &&  c <= KEYBOARD_KEY_NUMPAD9_DOWN )
						c = '0' + (char)(e - KEYBOARD_KEY_NUMPAD0_DOWN); 
					else if ( e == KEYBOARD_KEY_SPACE_DOWN )
						c = ' ';


					if ( c != '\0' )
					{
						currentText.insert(caret,1,c);
						set_text(currentText);
						moveCaret(+1);
					}
				}
					
				break;
			}
		}

		//if the value changed..
		if( currentText != lastText ){
			//Message m(5);
			Message m(3, 0, 2);
			m.add_integral_data(0, 0);
			m.add_integral_data(1, 0);
			m.add_integral_data(2, get_id());
			
			m.add_pointer(0, &lastText);
			m.add_pointer(1, &currentText);
			on_event(UIManager::get().get_state(),COMPONENT_ON_VALUE_CHANGED, m);
		}
	}
}

void Textbox::renderCaret(const float paddingX, const float dx, const float letterSize, const unsigned int currentLine, const unsigned int xFactor) const{
	//Draw caret
	if(m_hasFocus && m_showCaret && is_editable()){
		float x = get_x();
		float w = get_width();
		float h = get_height();
		float y = get_y();
		float px = paddingX + x + xFactor*dx;

		if(px + m_letterSpace*0.75f > x + w)
			px = x + w - m_letterSpace*0.75f;
		float caret_width = 0.01f;
		if(m_multiline){
			UIManager::get().render_box(px, y+(h-letterSize-dx*currentLine)-m_letterSpace, caret_width*0.75f, letterSize, m_caret_color);
		}else{
			UIManager::get().render_box(px, y+(h-letterSize)/2.0f, caret_width*0.75f, letterSize, m_caret_color);
		}
	}
}

void Textbox::render() const{
	if(!is_visible())
		return;

	float x = get_x();
	float y = get_y();
	float w = get_width();
	float h = get_height();

	UIManager::get().render_box(x, y, w, h, get_background_color());

	//Draw text
	float dx = m_letter_height + m_letterSpace;
	float paddingX = compute_padding_x(m_letter_height);
	
	if(paddingX < 0 )//if there isn't enough space in the texbox to contain all the text OR in multiline mode (because then we don't care if the text is centered), use an arbitrary amount
		paddingX = m_letterSpace;

	//Caret timing stuff
	double now = m_timer.get_time_in_mlsec();
	if(m_oldTime+m_blinkDelay <= now){
		//blink!
		m_showCaret = !m_showCaret;
		m_oldTime = now;
	}


	unsigned int display_chars = visible_characters();
	unsigned int display_lines = min(m_lines.size(),visible_lines());

	
	//multiline, handling warping and no warping
	if ( m_multiline )
	{
		unsigned long accumulated_chars = 0;



		caretRow = 0;
		caretCol = 0;

		for ( unsigned int i = 0;  i < display_lines;  ++i )
		{
			float y_pos = y + h - dx*(i+1);


			display_chars = min(m_lines[i].size(),visible_characters());
			caretCol = 0;

			for ( unsigned int j = 0;  j < m_lines[i].size();  ++j )
			{
				float x_pos = x + dx*j;
				render_character(m_lines[i][j],x_pos,y_pos,m_letter_height,m_letter_height);

				++accumulated_chars;

				if ( accumulated_chars < m_caretPosition )
					++caretCol;
			}

			++accumulated_chars;

			if ( accumulated_chars < m_caretPosition )
				++caretRow;
		}

		renderCaret(paddingX, dx, m_letter_height, caretRow, caretCol);
	}
	else
	{// =>if this is a single line box

		if ( m_lines.size() > 0 )
		{
			display_chars = min(static_cast<unsigned int>(m_lines[0].size()),display_chars);

			for ( unsigned int i = 0;  i < display_chars;  ++i )
			{
				float x_pos = x + dx*i;
				
				render_character(m_lines[0][i],x_pos,y,m_letter_height,m_letter_height);
			}
		}

		//Draw caret
		renderCaret(paddingX,dx,m_letter_height,0,m_caretPosition);
	}

	//Draw outline if needed
	if(m_render_border){
		render_border();
	}
}

Textbox::~Textbox(){
	m_timer.stop();
}

void Textbox::refactor_lines( const std::string& text )
{
	unsigned int chars_per_line = visible_characters();



	m_lines.clear();
	m_lines.push_back(text);

	if ( m_multiline )
	{
		for ( unsigned int i = 0;  i < m_lines.size();  ++i )
		{
			size_t	new_line_position = m_lines[i].find_first_of('\n');


			if ( new_line_position != m_lines[i].npos )
			{
				std::string new_line(m_lines[i].substr(new_line_position+1));


				if ( new_line != "" )
				{
					m_lines.push_back(new_line);
					m_lines[i] = m_lines[i].substr(0,new_line_position);
				}
			}

		}

		for ( unsigned int i = 0;  i < m_lines.size();  ++i )
		{
			bool	entered = false;



			if ( m_wrap  &&  m_lines[i].size() > chars_per_line )
			{
				size_t first_space = m_lines[i].find_first_of(" \t");
				size_t second_space = first_space;


				while ( second_space < m_lines[i].npos )
				{
					second_space = m_lines[i].find_first_of(" \t",first_space+1);

					if ( second_space != m_lines[i].npos  &&  second_space <= chars_per_line )
						first_space = second_space;
					else
						second_space = m_lines[i].npos;
				}

				if ( first_space != m_lines[i].npos )
				{
					std::string new_line(m_lines[i].substr(first_space+1));


					entered = true;
					if ( new_line != "" )
					{
						if ( (i+1) < m_lines.size() )
						{
							std::vector<std::string>::iterator it = m_lines.begin();


							for ( unsigned int j = 0;  j < (i+1); ++j )
								++it;

							m_lines.insert(it,new_line);
						}
						else
							m_lines.push_back(new_line);

						m_lines[i] = m_lines[i].substr(0,first_space+1);
					}

				}
			}
		
			if ( m_lines[i].size() > chars_per_line  &&  !entered )
			{
				std::string new_line(m_lines[i].substr(chars_per_line));


				if ( new_line != "" )
				{
					if ( (i+1) < m_lines.size() )
					{
						std::vector<std::string>::iterator it = m_lines.begin();


						for ( unsigned int j = 0;  j < (i+1); ++j )
							++it;

						m_lines.insert(it,new_line);
					}
					else
						m_lines.push_back(new_line);

					m_lines[i] = m_lines[i].substr(0,chars_per_line);
				}
			}
		}
	}
}

	}//namespace UI
}//namespace TEXEngine
