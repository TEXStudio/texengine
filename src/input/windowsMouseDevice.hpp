#include	"inputDevice.hpp"
#ifndef		WIN32_LEAN_AND_MEAN
	#define	WIN32_LEAN_AND_MEAN
	#define	WIN64_LEAN_AND_MEAN
	#define	NOMINMAX
#endif		/* WIN32_LEAN_AND_MEAN */	
#include	<Windows.h>

#ifdef			_WIN32
	#ifndef		TEXENGINE_WINDOWS_MOUSE_DEVICE_HPP_INCLUDED
		#define	TEXENGINE_WINDOWS_MOUSE_DEVICE_HPP_INCLUDED



		namespace	TEXEngine
		{
			namespace	Device
			{

				/*
					Class responsible for handling input from a mouse device
				*/
				class	WindowsMouseDevice	:	public InputDevice
				{
					private:

						static	const unsigned int	_mouse_buttons = 5;
						static	const unsigned int	_keyboard_buttons = 256;

						SHORT						_current_mouse_button_state[_mouse_buttons];
						SHORT						_previous_mouse_button_state[_mouse_buttons];
						POINT						_current_position;
						POINT						_previous_position;
						HWND						_window_handle;
						bool						_switched;
					

					public:

						WindowsMouseDevice( HWND handle = NULL );
						~WindowsMouseDevice();


						void						initialise();
						void						update( const unsigned long id );
						void						terminate();

						void						handle( HWND id );
						HWND						handle();
				};



				/*
					Function Implementations
				*/

				inline	WindowsMouseDevice::WindowsMouseDevice( HWND handle)
					:	_window_handle(handle) , _switched(false)		{};
				inline	WindowsMouseDevice::~WindowsMouseDevice()		{ this->terminate(); };

				inline void	WindowsMouseDevice::handle( HWND handle )	{ this->_window_handle = handle; };
				inline HWND	WindowsMouseDevice::handle()				{ return this->_window_handle; };

			}	/* Device */
		}	/* TEXEngine */



	#endif		/* TEXENGINE_WINDOWS_MOUSE_DEVICE_HPP_INCLUDED */
#endif			/* _WIN32 */