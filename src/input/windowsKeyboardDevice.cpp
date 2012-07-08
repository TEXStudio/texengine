#ifdef		_WIN32

	#include	<cstring>
	#include	"windowsKeyboardDevice.hpp"
	#include	"..\message\TEXEngineInterface.hpp"



	namespace	TEXEngine
	{
		namespace	Device
		{

			void	WindowsKeyboardDevice::initialise()
			{
				std::memset(this->_current_key_state,'\0',this->_keyboard_buttons*sizeof(BYTE));
				std::memset(this->_previous_key_state,'\0',this->_keyboard_buttons*sizeof(BYTE));
				SetKeyboardState(this->_current_key_state);
			};

			void	WindowsKeyboardDevice::update( const unsigned long id )
			{
				HWND						active_window = GetForegroundWindow();
				unsigned long				active_id = GetWindowThreadProcessId(active_window,NULL);
				unsigned long				this_id = GetCurrentThreadId();
				BOOL						succeeded = FALSE;
				Core::Message				parameters(2,0,0);
		


				parameters.add_integral_data(0,static_cast<Core::MESSAGE_INTEGRAL_DATA_TYPE>(id));
				std::memcpy(this->_previous_key_state,this->_current_key_state,_keyboard_buttons*sizeof(BYTE));


				if ( this->_window_handle == active_window )
				{
					BOOL	attached = AttachThreadInput(this_id,active_id,TRUE);



					if ( this->_switched )
					{
						std::memset(this->_previous_key_state,'\0',this->_keyboard_buttons*sizeof(BYTE));
						SetKeyboardState(this->_previous_key_state);
						this->_switched = false;
					}
						
					succeeded = GetKeyboardState(this->_current_key_state);
					
					if ( attached == TRUE )
						AttachThreadInput(this_id,active_id,FALSE);
				}
				else
				{
					if ( this->_switched == false )
					{
						std::memset(this->_current_key_state,'\0',this->_keyboard_buttons*sizeof(BYTE));
						succeeded = TRUE;
					}

					this->_switched = true;
				}


				if ( succeeded )
				{
					for ( unsigned int i = 0x08;  i < 0xFE;  ++i )
					{
						int	difference = this->_current_key_state[i] - this->_previous_key_state[i];
			

						if ( difference < 0 )
							trigger_event(Core::KEYBOARD_KEY_BACKSPACE_UP+(i-0x08),parameters);
						else if ( difference > 0 )
							trigger_event(Core::KEYBOARD_KEY_BACKSPACE_DOWN+(i-0x08),parameters);
					}
				}
			};

			void	WindowsKeyboardDevice::terminate()
			{
			};

		}	/* Device */
	}	/* TEXEngine */



#endif		/* _WIN32 */