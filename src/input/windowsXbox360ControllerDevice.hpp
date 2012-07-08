#include	"inputDevice.hpp"
#ifndef		WIN32_LEAN_AND_MEAN
	#define	WIN32_LEAN_AND_MEAN
	#define	WIN64_LEAN_AND_MEAN
	#define	NOMINMAX
#endif		/* WIN32_LEAN_AND_MEAN */	
#include	<Windows.h>

#ifdef			_WIN32
	#ifndef		TEXENGINE_WINDOWS_XBOX360_CONTROLLER_DEVICE_HPP_INCLUDED
		#define	TEXENGINE_WINDOWS_XBOX360_CONTROLLER_DEVICE_HPP_INCLUDED

		#include	<Xinput.h>
		#pragma		comment(lib,"Xinput.lib")



		namespace	TEXEngine
		{
			namespace	Device
			{

				/*
					Class responsible for handling input from a XBox 360 controller
				*/
				class	WindowsXbox360ControllerDevice	:	public InputDevice
				{
					private:

						static const unsigned long	_deadzone = 3;
						unsigned long				_id;
						XINPUT_STATE				_current_state;
						XINPUT_STATE				_previous_state;
						HWND						_window_handle;


					public:

						WindowsXbox360ControllerDevice( const unsigned long id = 0 , HWND handle = NULL );
						~WindowsXbox360ControllerDevice();


						void						initialise();
						void						update( const unsigned long id );
						void						terminate();

						void						handle( HWND id );
						HWND						handle();
				};



				/*
					Function Implementations 
				*/


				inline	WindowsXbox360ControllerDevice::WindowsXbox360ControllerDevice( const unsigned long id , HWND handle)	:	
					_id(id) , _window_handle(handle)																			{};
				inline	WindowsXbox360ControllerDevice::~WindowsXbox360ControllerDevice()										{ this->terminate(); };


				inline void	WindowsXbox360ControllerDevice::handle( HWND handle )												{ this->_window_handle = handle; };
				inline HWND	WindowsXbox360ControllerDevice::handle()															{ return this->_window_handle; };

			}	/* Device */
		}	/* TEXEngine */

	

	#endif		/* TEXENGINE_WINDOWS_XBOX360_CONTROLLER_DEVICE_HPP_INCLUDED */
#endif			/* _WIN32 */