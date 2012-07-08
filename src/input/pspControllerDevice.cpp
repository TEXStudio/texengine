#if defined(SN_TARGET_PSP_HW) || defined(SN_TARGET_PSP_PRX)
	
	#include	<cstring>
	#include	<cstdlib>
	#include	"pspControllerDevice.hpp"
	#include	"..\message\TEXEngineInterface.hpp"
	


	namespace	TEXEngine
	{
		namespace	Device
		{
	
			void	PSPControllerDevice::initialise()
			{
				sceCtrlSetSamplingMode(SCE_CTRL_MODE_DIGITALANALOG);
				std::memset(&this->_current_button_state,'\0',sizeof(SceCtrlData));
				std::memset(&this->_previous_button_state,'\0',sizeof(SceCtrlData));
			};

			void	PSPControllerDevice::update( const unsigned long id )
			{
				Core::Message	parameters(5,0,0);
				int				difference_x = 0;
				int				difference_y = 0;
		
		
		
				parameters.add_integral_data(0,id);
				parameters.add_integral_data(2,0);
				memcpy(&this->_previous_button_state,&this->_current_button_state,sizeof(SceCtrlData));
				sceCtrlPeekBufferPositive(&this->_current_button_state,1);


				parameters.add_integral_data(3,static_cast<Core::MESSAGE_INTEGRAL_DATA_TYPE>(this->_current_button_state.Lx));
				parameters.add_integral_data(4,static_cast<Core::MESSAGE_INTEGRAL_DATA_TYPE>(this->_current_button_state.Ly));
				trigger_event(Core::PSP_CONTROLLER_STICK_POSITION,parameters);


				difference_x = this->_current_button_state.Lx - this->_previous_button_state.Lx; 
				difference_y = this->_current_button_state.Ly - this->_previous_button_state.Ly;
			
				if ( std::abs(difference_x) > _deadzone  ||  std::abs(difference_y) > _deadzone )
				{
					parameters.add_integral_data(3,static_cast<Core::MESSAGE_INTEGRAL_DATA_TYPE>(difference_x));
					parameters.add_integral_data(4,static_cast<Core::MESSAGE_INTEGRAL_DATA_TYPE>(difference_y));
					trigger_event(Core::PSP_CONTROLLER_STICK_POSITION_CHANGED,parameters);
				}

				if ( this->_current_button_state.Buttons != this->_previous_button_state.Buttons )
				{
					int	select = (this->_current_button_state.Buttons & SCE_CTRL_SELECT)  - (this->_previous_button_state.Buttons & SCE_CTRL_SELECT);
					int	start = (this->_current_button_state.Buttons & SCE_CTRL_START) - (this->_previous_button_state.Buttons & SCE_CTRL_START);
					int	up = (this->_current_button_state.Buttons & SCE_CTRL_UP) -( this->_previous_button_state.Buttons & SCE_CTRL_UP);
					int	right = (this->_current_button_state.Buttons & SCE_CTRL_RIGHT) - (this->_previous_button_state.Buttons & SCE_CTRL_RIGHT);
					int	down = (this->_current_button_state.Buttons & SCE_CTRL_DOWN) - (this->_previous_button_state.Buttons & SCE_CTRL_DOWN);
					int	left = (this->_current_button_state.Buttons & SCE_CTRL_LEFT) - (this->_previous_button_state.Buttons & SCE_CTRL_LEFT);
					int	L = (this->_current_button_state.Buttons & SCE_CTRL_L) - (this->_previous_button_state.Buttons & SCE_CTRL_L);
					int	R = (this->_current_button_state.Buttons & SCE_CTRL_R) - (this->_previous_button_state.Buttons & SCE_CTRL_R);
					int	triangle = (this->_current_button_state.Buttons & SCE_CTRL_TRIANGLE) - (this->_previous_button_state.Buttons & SCE_CTRL_TRIANGLE);
					int	circle = (this->_current_button_state.Buttons & SCE_CTRL_CIRCLE) - (this->_previous_button_state.Buttons & SCE_CTRL_CIRCLE);
					int	cross = (this->_current_button_state.Buttons & SCE_CTRL_CROSS) - (this->_previous_button_state.Buttons & SCE_CTRL_CROSS);
					int	square= (this->_current_button_state.Buttons & SCE_CTRL_SQUARE) - (this->_previous_button_state.Buttons & SCE_CTRL_SQUARE);



					parameters.add_integral_data(3,0);
					parameters.add_integral_data(4,0);
					if ( select < 0 )
						trigger_event(Core::PSP_CONTROLLER_SELECT_UP,parameters);
					else if ( select > 0 )
						trigger_event(Core::PSP_CONTROLLER_SELECT_DOWN,parameters);

					if ( start < 0 )
						trigger_event(Core::PSP_CONTROLLER_START_UP,parameters);
					else if ( start > 0 )
						trigger_event(Core::PSP_CONTROLLER_START_DOWN,parameters);

					if ( up < 0 )
						trigger_event(Core::PSP_CONTROLLER_UPARROW_UP,parameters);
					else if ( up > 0 )
						trigger_event(Core::PSP_CONTROLLER_UPARROW_DOWN,parameters);

					if ( right < 0 )
						trigger_event(Core::PSP_CONTROLLER_RIGHTARROW_UP,parameters);
					else if ( right > 0 )
						trigger_event(Core::PSP_CONTROLLER_RIGHTARROW_DOWN,parameters);

					if ( down < 0 )
						trigger_event(Core::PSP_CONTROLLER_DOWNARROW_UP,parameters);
					else if ( down > 0 )
						trigger_event(Core::PSP_CONTROLLER_DOWNARROW_DOWN,parameters);

					if ( left < 0 )
						trigger_event(Core::PSP_CONTROLLER_LEFTARROW_UP,parameters);
					else if ( left > 0 )
						trigger_event(Core::PSP_CONTROLLER_LEFTARROW_DOWN,parameters);

					if ( L < 0 )
						trigger_event(Core::PSP_CONTROLLER_LEFTTRIGGER_UP,parameters);
					else if ( L > 0 )
						trigger_event(Core::PSP_CONTROLLER_LEFTTRIGGER_DOWN,parameters);
			
					if ( R < 0 )
						trigger_event(Core::PSP_CONTROLLER_RIGHTTRIGGER_UP,parameters);
					else if ( R > 0 )
						trigger_event(Core::PSP_CONTROLLER_RIGHTTRIGGER_DOWN,parameters);

					if ( triangle < 0 )
						trigger_event(Core::PSP_CONTROLLER_TRIANGLE_UP,parameters);
					else if ( triangle > 0 )
						trigger_event(Core::PSP_CONTROLLER_TRIANGLE_DOWN,parameters);

					if ( circle < 0 )
						trigger_event(Core::PSP_CONTROLLER_CIRCLE_UP,parameters);
					else if ( circle > 0 )
						trigger_event(Core::PSP_CONTROLLER_CIRCLE_DOWN,parameters);

					if ( cross < 0 )
						trigger_event(Core::PSP_CONTROLLER_CROSS_UP,parameters);
					else if ( cross > 0 )
						trigger_event(Core::PSP_CONTROLLER_CROSS_DOWN,parameters);

					if ( square < 0 )
						trigger_event(Core::PSP_CONTROLLER_SQUARE_UP,parameters);
					else if ( square > 0 )
						trigger_event(Core::PSP_CONTROLLER_SQUARE_DOWN,parameters);
				}
			};

			void	PSPControllerDevice::terminate()
			{
			};
		
		}	/* Device */
	}	/* TEXEngine */

#endif	/* SN_TARGET_PSP_HW || SN_TARGET_PSP_PRX */