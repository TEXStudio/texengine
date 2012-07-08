#ifdef		_WIN32

	#include	<cstring>
	#include	"windowsMouseDevice.hpp"
	#include	"..\message\TEXEngineInterface.hpp"



	namespace	TEXEngine
	{
		namespace	Device
		{

			void	WindowsMouseDevice::initialise()
			{
				std::memset(&this->_current_position,'\0',sizeof(POINT));
				std::memset(&this->_previous_position,'\0',sizeof(POINT));
				std::memset(&this->_current_mouse_button_state,'\0',this->_mouse_buttons*sizeof(SHORT));
				std::memset(&this->_previous_mouse_button_state,'\0',this->_mouse_buttons*sizeof(SHORT));
			};

			void	WindowsMouseDevice::update( const unsigned long id )
			{
				CURSORINFO					cursor_info;
				HWND						active_window = GetForegroundWindow();
				unsigned long				active_id = GetWindowThreadProcessId(active_window,NULL);
				unsigned long				this_id = GetCurrentThreadId();
				Core::Message				parameters(4,0,0);
				bool						succeeded = false;
		


				parameters.add_integral_data(0,id);
				memcpy(&this->_previous_position,&this->_current_position,sizeof(POINT));
				memcpy(&this->_previous_mouse_button_state,&this->_current_mouse_button_state,_mouse_buttons*sizeof(SHORT));


				if ( this->_window_handle == active_window )
				{
					BOOL	attached = AttachThreadInput(this_id,active_id,TRUE);



					if ( this->_switched == true )
					{
						BYTE	current_key_state[this->_keyboard_buttons];



						std::memset(current_key_state,'\0',this->_keyboard_buttons*sizeof(BYTE));
						std::memset(this->_previous_mouse_button_state,'\0',this->_mouse_buttons*sizeof(SHORT));
						GetKeyboardState(current_key_state);

						current_key_state[VK_LBUTTON] = 0;
						current_key_state[VK_RBUTTON] = 0;
						current_key_state[VK_MBUTTON] = 0;
						current_key_state[VK_XBUTTON1] = 0;
						current_key_state[VK_XBUTTON2] = 0;

						SetKeyboardState(current_key_state);
						this->_switched = false;
					}



					this->_current_mouse_button_state[0] = GetKeyState(VK_LBUTTON);
					this->_current_mouse_button_state[1] = GetKeyState(VK_RBUTTON);
					this->_current_mouse_button_state[2] = GetKeyState(VK_MBUTTON);
					this->_current_mouse_button_state[3] = GetKeyState(VK_XBUTTON1);
					this->_current_mouse_button_state[4] = GetKeyState(VK_XBUTTON2);
					succeeded = true;

					if ( attached == TRUE  )
						AttachThreadInput(this_id,active_id,FALSE);


					cursor_info.cbSize = sizeof(CURSORINFO);
					 if ( GetCursorInfo(&cursor_info) )
					 {
						memcpy(&this->_current_position,&cursor_info.ptScreenPos,sizeof(POINT));
						parameters.add_integral_data(2,static_cast<Core::MESSAGE_INTEGRAL_DATA_TYPE>(this->_current_position.x));
						parameters.add_integral_data(3,static_cast<Core::MESSAGE_INTEGRAL_DATA_TYPE>(this->_current_position.y));
						trigger_event(Core::MOUSE_POSITION,parameters);


						long	difference_x = this->_current_position.x - this->_previous_position.x;
						long	difference_y = this->_current_position.y - this->_previous_position.y;



						if ( difference_x != 0  ||  difference_y != 0 )
						{
							parameters.add_integral_data(2,static_cast<Core::MESSAGE_INTEGRAL_DATA_TYPE>(difference_x));
							parameters.add_integral_data(3,static_cast<Core::MESSAGE_INTEGRAL_DATA_TYPE>(difference_y));
							trigger_event(Core::MOUSE_POSITION_CHANGED,parameters);
						}
					 }
				}
				else
				{
					if ( this->_switched == false )
					{
						std::memset(this->_current_mouse_button_state,'\0',this->_mouse_buttons*sizeof(SHORT));
						succeeded = true;
					}

					this->_switched = true;
				}


				if ( succeeded )
				{
					for ( unsigned int i = 0;  i < this->_mouse_buttons;  ++i )
					{
						int	difference = HIWORD(this->_current_mouse_button_state[i]) - HIWORD(this->_previous_mouse_button_state[i]);



						if ( difference < 0 )
							trigger_event(Core::MOUSE_BUTTON_LEFT_UP+i,parameters);
						else if ( difference > 0 )
							trigger_event(Core::MOUSE_BUTTON_LEFT_DOWN+i,parameters);
					}
				}
			};

			void	WindowsMouseDevice::terminate()
			{
			};
		}	/* Device */
	}	/* TEXEngine */



#endif		/* _WIN32 */