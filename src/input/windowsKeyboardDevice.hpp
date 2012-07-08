#include	"inputDevice.hpp"
#ifndef		WIN32_LEAN_AND_MEAN
	#define	WIN32_LEAN_AND_MEAN
	#define	WIN64_LEAN_AND_MEAN
	#define	NOMINMAX
#endif		/* WIN32_LEAN_AND_MEAN */	
#include	<Windows.h>

#ifdef			_WIN32
	#ifndef		TEXENGINE_WINDOWS_KEYBOARD_DEVICE_HPP_INCLUDED
		#define	TEXENGINE_WINDOWS_KEYBOARD_DEVICE_HPP_INCLUDED



		namespace	TEXEngine
		{
			namespace	Device
			{

				/*
					Class responsible for handling input from the keyboard
				*/
				class	WindowsKeyboardDevice	:	public InputDevice
				{
					private:

						static	const unsigned int	_keyboard_buttons = 256;
						BYTE						_current_key_state[_keyboard_buttons];
						BYTE						_previous_key_state[_keyboard_buttons];
						HWND						_window_handle;
						bool						_switched;


					public:

						WindowsKeyboardDevice( HWND handle = NULL );
						~WindowsKeyboardDevice();


						void						initialise();
						void						update( const unsigned long id );
						void						terminate();

						void						handle( HWND id );
						HWND						handle();
				};



				/*
					Function implementations
				*/

				inline	WindowsKeyboardDevice::WindowsKeyboardDevice( HWND handle )
					:	_window_handle(handle) , _switched(false)			{};
				inline	WindowsKeyboardDevice::~WindowsKeyboardDevice()		{ this->terminate(); };


				inline void	WindowsKeyboardDevice::handle( HWND handle )	{ this->_window_handle = handle; };
				inline HWND	WindowsKeyboardDevice::handle()					{ return this->_window_handle; };

			}	/* Device */
		}	/* TEXEngine */



	#endif		/* TEXENGINE_WINDOWS_KEYBOARD_DEVICE_HPP_INCLUDED */
#endif			/* _WIN32 */