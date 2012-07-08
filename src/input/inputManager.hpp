#include	<vector>
#include	<cstring>
#include	"..\message\TEXEngineInterface.hpp"
#include	"..\message\object.hpp"
#include	"..\message\message.hpp"
#include	"inputDevice.hpp"

#ifdef		_WIN32
	#include	"windowsKeyboardDevice.hpp"
	#include	"windowsMouseDevice.hpp"
	#include	"windowsXbox360ControllerDevice.hpp"
#endif

#ifndef		TEXENGINE_INPUT_MANAGER_HPP_INCLUDED
	#define	TEXENGINE_INPUT_MANAGER_HPP_INCLUDED



	namespace	TEXEngine
	{
		namespace	Core
		{

			/*
				Class responsible for handling input devices and parsing input
			*/
			class	InputManager	:	public Object
			{
				private:
				
					static	InputManager				_manager;
				
					#ifdef	_WIN32
						static	const unsigned int		_max_xbox360_controllers = 4;
					#endif	/* _WIN32 */

					std::vector<Device::InputDevice*>	_devices;
					unsigned long						_update_rate;

					#ifdef	_WIN32
								HWND					_window_handle;
								bool					_keyboard;
								bool					_mouse;
								bool					_xbox360;
					#endif	/* _WIN32 */
									
					InputManager();
					~InputManager();

					void								_initialise_input_device( const unsigned long event_code , const Message& parameters );
					void								_terminate_input_device( std::vector<Device::InputDevice*>::iterator device );
					
					#ifdef	_WIN32
						void							_initialise( HWND handle );
					#else
						void							_initialise();
					#endif	/* _WIN32 */

					void								_terminate();
					void								_change_update_rate( const unsigned long event_code );
					void								_update();


				public:

					//	Function to get the single instance of this class.
					static InputManager&				get();
				
				
					void								setup();
					//	Function called by the MessageManager in order to respond to the specified events.	
					void								on_event( const unsigned long event_code , const Message& parameters );
			};



			/*
				Function Implementations
			*/
		
			inline	InputManager::InputManager()	:	
				Object(2) , _update_rate(UPDATE) 
				#ifdef	_WIN32
					, _keyboard(false) , _mouse(false) , _xbox360(false)
				#endif	/* _WIN32 */
															{};
			inline	InputManager::~InputManager()			{ this->_terminate(); };
	

			inline InputManager&	InputManager::get()		{ return _manager; };

		}	/* Core */
	}	/* TEXEngine */



#endif		/* TEXENGINE_INPUT_MANAGER_HPP_INCLUDED */