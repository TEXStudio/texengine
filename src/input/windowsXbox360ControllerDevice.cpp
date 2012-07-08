#ifdef	_WIN32

	#include	<cstdlib>
	#include	<cstring>
	#include	"windowsXbox360ControllerDevice.hpp"
	#include	"..\message\TEXEngineInterface.hpp"



	namespace	TEXEngine
	{
		namespace	Device
		{

			void	WindowsXbox360ControllerDevice::initialise()
			{
				memset(&this->_current_state,'\0',sizeof(XINPUT_STATE));
				memset(&this->_previous_state,'\0',sizeof(XINPUT_STATE));
			};

			void	WindowsXbox360ControllerDevice::update( const unsigned long id )
			{
				Core::Message	parameters(5,0,0);
				HWND			foreground = GetForegroundWindow();



				if ( this->_window_handle == foreground )
				{
					parameters.add_integral_data(0,id);
					parameters.add_integral_data(2,this->_id);
					memcpy(&this->_previous_state,&this->_current_state,sizeof(XINPUT_STATE));

					if ( XInputGetState(static_cast<DWORD>(this->_id),&this->_current_state)  ==  ERROR_SUCCESS )
					{
						parameters.add_integral_data(3,static_cast<Core::MESSAGE_INTEGRAL_DATA_TYPE>(this->_current_state.Gamepad.sThumbLX));
						parameters.add_integral_data(4,static_cast<Core::MESSAGE_INTEGRAL_DATA_TYPE>(this->_current_state.Gamepad.sThumbLY));
						trigger_event(Core::XBOX360_CONTROLLER_LEFTSTICK_POSITION,parameters);

						parameters.add_integral_data(3,static_cast<Core::MESSAGE_INTEGRAL_DATA_TYPE>(this->_current_state.Gamepad.sThumbRX));
						parameters.add_integral_data(4,static_cast<Core::MESSAGE_INTEGRAL_DATA_TYPE>(this->_current_state.Gamepad.sThumbRY));
						trigger_event(Core::XBOX360_CONTROLLER_RIGHTSTICK_POSITION,parameters);

						if ( this->_previous_state.dwPacketNumber != this->_current_state.dwPacketNumber )
						{
							int	difference_left_x = this->_current_state.Gamepad.sThumbLX - this->_previous_state.Gamepad.sThumbLX;
							int	difference_left_y = this->_current_state.Gamepad.sThumbLY - this->_previous_state.Gamepad.sThumbLY;
							int	difference_right_x = this->_current_state.Gamepad.sThumbRX - this->_previous_state.Gamepad.sThumbRX;
							int	difference_right_y = this->_current_state.Gamepad.sThumbRY - this->_previous_state.Gamepad.sThumbRY;



							if ( abs(difference_left_x) > this->_deadzone  ||  abs(difference_left_y) > this->_deadzone )
							{
								parameters.add_integral_data(3,static_cast<Core::MESSAGE_INTEGRAL_DATA_TYPE>(difference_left_x));
								parameters.add_integral_data(4,static_cast<Core::MESSAGE_INTEGRAL_DATA_TYPE>(difference_left_y));
								trigger_event(Core::XBOX360_CONTROLLER_LEFTSTICK_POSITION_CHANGED,parameters);
							}

							if ( abs(difference_right_x) > this->_deadzone  ||  abs(difference_right_y) > this->_deadzone )
							{
								parameters.add_integral_data(3,static_cast<Core::MESSAGE_INTEGRAL_DATA_TYPE>(difference_right_x));
								parameters.add_integral_data(4,static_cast<Core::MESSAGE_INTEGRAL_DATA_TYPE>(difference_right_y));
								trigger_event(Core::XBOX360_CONTROLLER_RIGHTSTICK_POSITION_CHANGED,parameters);
							}


							int	up = (this->_current_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) - (this->_previous_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP);
							int	down = (this->_current_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) - (this->_previous_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
							int	left = (this->_current_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) - (this->_previous_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
							int	right = (this->_current_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) - (this->_previous_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
							int	start = (this->_current_state.Gamepad.wButtons & XINPUT_GAMEPAD_START) - (this->_previous_state.Gamepad.wButtons & XINPUT_GAMEPAD_START);
							int	back = (this->_current_state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) - (this->_previous_state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK);
							int	left_thumb = (this->_current_state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) - (this->_previous_state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
							int	right_thumb = (this->_current_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) - (this->_previous_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);
							int	left_shoulder = (this->_current_state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) - (this->_previous_state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
							int	right_shoulder = (this->_current_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) - (this->_previous_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
							int	a = (this->_current_state.Gamepad.wButtons & XINPUT_GAMEPAD_A) - (this->_previous_state.Gamepad.wButtons & XINPUT_GAMEPAD_A);
							int	b = (this->_current_state.Gamepad.wButtons & XINPUT_GAMEPAD_B) - (this->_previous_state.Gamepad.wButtons & XINPUT_GAMEPAD_B);
							int	x = (this->_current_state.Gamepad.wButtons & XINPUT_GAMEPAD_X) - (this->_previous_state.Gamepad.wButtons & XINPUT_GAMEPAD_X);
							int	y = (this->_current_state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) - (this->_previous_state.Gamepad.wButtons & XINPUT_GAMEPAD_Y);
							int	left_trigger = this->_current_state.Gamepad.bLeftTrigger - this->_previous_state.Gamepad.bLeftTrigger;
							int	right_trigger = this->_current_state.Gamepad.bRightTrigger - this->_previous_state.Gamepad.bRightTrigger;



							parameters.add_integral_data(3,0);
							parameters.add_integral_data(4,0);

							if ( up < 0 )
								trigger_event(Core::XBOX360_CONTROLLER_DPAD_UP_UP,parameters);
							else if ( up > 0 )
								trigger_event(Core::XBOX360_CONTROLLER_DPAD_UP_DOWN,parameters);

							if ( down < 0 )
								trigger_event(Core::XBOX360_CONTROLLER_DPAD_DOWN_UP,parameters);
							else if ( down > 0 )
								trigger_event(Core::XBOX360_CONTROLLER_DPAD_DOWN_DOWN,parameters);

							if ( left < 0 )
								trigger_event(Core::XBOX360_CONTROLLER_DPAD_LEFT_UP,parameters);
							else if ( left > 0 )
								trigger_event(Core::XBOX360_CONTROLLER_DPAD_LEFT_DOWN,parameters);

							if ( right < 0 )
								trigger_event(Core::XBOX360_CONTROLLER_DPAD_RIGHT_UP,parameters);
							else if ( right > 0 )
								trigger_event(Core::XBOX360_CONTROLLER_DPAD_RIGHT_DOWN,parameters);

							if ( start < 0 )
								trigger_event(Core::XBOX360_CONTROLLER_START_UP,parameters);
							else if ( start > 0 )
								trigger_event(Core::XBOX360_CONTROLLER_START_DOWN,parameters);

							if ( back < 0 )
								trigger_event(Core::XBOX360_CONTROLLER_BACK_UP,parameters);
							else if ( back > 0 )
								trigger_event(Core::XBOX360_CONTROLLER_BACK_DOWN,parameters);

							if ( left_thumb < 0 )
								trigger_event(Core::XBOX360_CONTROLLER_LEFTSTICK_UP,parameters);
							else if ( left_thumb > 0 )
								trigger_event(Core::XBOX360_CONTROLLER_LEFTSTICK_DOWN,parameters);

							if ( right_thumb < 0 )
								trigger_event(Core::XBOX360_CONTROLLER_RIGHTSTICK_UP,parameters);
							else if ( right_thumb > 0 )
								trigger_event(Core::XBOX360_CONTROLLER_RIGHTSTICK_DOWN,parameters);

							if ( left_shoulder < 0 )
								trigger_event(Core::XBOX360_CONTROLLER_LEFTSHOULDER_UP,parameters);
							else if ( left_shoulder > 0 )
								trigger_event(Core::XBOX360_CONTROLLER_LEFTSHOULDER_DOWN,parameters);

							if ( right_shoulder < 0 )
								trigger_event(Core::XBOX360_CONTROLLER_RIGHTSHOULDER_UP,parameters);
							else if ( right_shoulder > 0 )
								trigger_event(Core::XBOX360_CONTROLLER_RIGHTSHOULDER_DOWN,parameters);

							if ( a < 0 )
								trigger_event(Core::XBOX360_CONTROLLER_A_UP,parameters);
							else if ( a > 0 )
								trigger_event(Core::XBOX360_CONTROLLER_A_DOWN,parameters);

							if ( b < 0 )
								trigger_event(Core::XBOX360_CONTROLLER_B_UP,parameters);
							else if ( b > 0 )
								trigger_event(Core::XBOX360_CONTROLLER_B_DOWN,parameters);

							if ( x < 0 )
								trigger_event(Core::XBOX360_CONTROLLER_X_UP,parameters);
							else if ( x > 0 )
								trigger_event(Core::XBOX360_CONTROLLER_X_DOWN,parameters);

							if ( y < 0 )
								trigger_event(Core::XBOX360_CONTROLLER_Y_UP,parameters);
							else if ( y > 0 )
								trigger_event(Core::XBOX360_CONTROLLER_Y_DOWN,parameters);


							parameters.add_integral_data(3,static_cast<Core::MESSAGE_INTEGRAL_DATA_TYPE>(left_trigger));
							if ( left_trigger < 0 )
								trigger_event(Core::XBOX360_CONTROLLER_LEFTTRIGGER_UP,parameters);
							else if ( left_trigger > 0 )
								trigger_event(Core::XBOX360_CONTROLLER_LEFTTRIGGER_DOWN,parameters);

							parameters.add_integral_data(3,static_cast<Core::MESSAGE_INTEGRAL_DATA_TYPE>(right_trigger));
							if ( right_trigger < 0 )
								trigger_event(Core::XBOX360_CONTROLLER_RIGHTTRIGGER_UP,parameters);
							else if ( right_trigger > 0 )
								trigger_event(Core::XBOX360_CONTROLLER_RIGHTTRIGGER_DOWN,parameters);
						}
					}
				}
			};

			void	WindowsXbox360ControllerDevice::terminate()
			{
			};

		}	/* Device */
	}	/* TEXEngine */



#endif	/* _WIN32 */