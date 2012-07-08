#include	"inputDevice.hpp"

#if defined(SN_TARGET_PSP_HW) || defined(SN_TARGET_PSP_PRX)
	#ifndef		TEXENGINE_PSP_CONTROLLER_DEVICE_HPP_INCLUDED
		#define	TEXENGINE_PSP_CONTROLLER_DEVICE_HPP_INCLUDED

		#include	<ctrlsvc.h>



		namespace	TEXEngine
		{
			namespace	Device
			{

				/*
					Class responsible for handling input from a PSP controller
				*/
				class	PSPControllerDevice	:	public InputDevice
				{
					private:

						static const unsigned int	_deadzone = 2;
						SceCtrlData					_current_button_state;
						SceCtrlData					_previous_button_state;


					public:

						PSPControllerDevice();
						~PSPControllerDevice();


						void						initialise();
						void						update( const unsigned long id );
						void						terminate();

				};
			
			
			
				/* 
					Function implementations#
				*/

				inline	PSPControllerDevice::PSPControllerDevice()	{};
				inline	PSPControllerDevice::~PSPControllerDevice()	{ this->terminate(); };

			}	/* Device */
		}	/* TEXEngine */



	#endif		/* TEXENGINE_PSP_CONTROLLER_DEVICE_HPP_INCLUDED */
#endif			/* SN_TARGET_PSP_HW || SN_TARGET_PSP_PRX */		