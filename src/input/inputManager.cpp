#include	"inputManager.hpp"



namespace	TEXEngine
{
	namespace	Core
	{

		InputManager	InputManager::_manager;


		void	InputManager::_initialise_input_device( const unsigned long event_code , const Message& parameters )
		{
			Device::InputDevice*	device = NULL;



			#ifdef	_WIN32
				if ( event_code == INIT_KEYBOARD  &&  !this->_keyboard )
				{
					device = new (std::nothrow) Device::WindowsKeyboardDevice(_window_handle);

					if ( device != NULL )
					{
						device->initialise();
						this->_devices.push_back(device);
						this->_keyboard = true;
					}
				}
				else if ( event_code == INIT_MOUSE  &&  !this->_mouse )
				{
					device = new (std::nothrow) Device::WindowsMouseDevice(_window_handle);

					if ( device != NULL )
					{
						device->initialise();
						this->_devices.push_back(device);
						this->_mouse = true;
					}
				}
				else if ( event_code == INIT_XBOX360_CONTROLLER  &&  !this->_xbox360 )
				{
					unsigned int	controllers = static_cast<unsigned int>(parameters.get_integral_data(2));



					if ( controllers > this->_max_xbox360_controllers )
						controllers = this->_max_xbox360_controllers;

					for ( unsigned int i = 0;  i < controllers;  ++i )
					{
						device = NULL;
						device = new (std::nothrow) Device::WindowsXbox360ControllerDevice(i,_window_handle);

						if ( device != NULL )
						{
							device->initialise();
							this->_devices.push_back(device);
							this->_xbox360 = true;
						}
					}
				}	
			#endif	/* _WIN32 */
		};

		void	InputManager::_terminate_input_device( std::vector<Device::InputDevice*>::iterator device )
		{
			delete (*device);
			(*device) = NULL;
		};

		#ifdef	_WIN32
			void	InputManager::_initialise( HWND handle )
		#else
			void	InputManager::_initialise()
		#endif	/* _WIN32 */
		{
			#ifdef	_WIN32
				if ( handle )
					this->_window_handle = handle;
			#endif	/* _WIN32 */
		}

		void	InputManager::_terminate()
		{
			for ( std::vector<Device::InputDevice*>::iterator device_iterator = this->_devices.begin();  device_iterator != this->_devices.end();  ++device_iterator )
				this->_terminate_input_device(device_iterator);

			this->_devices.clear();
		};

		void	InputManager::_change_update_rate( const unsigned long event_code )
		{
			if ( event_code != this->_update_rate )
			{
				if ( event_code >= UPDATE  &&  event_code <= UPDATE_SLOW  )
				{
					this->unregister_event(this->_update_rate);
					this->_update_rate = event_code;
					this->register_event(this->_update_rate);
				}
			}
		};

		void	InputManager::_update()
		{
			for ( std::vector<Device::InputDevice*>::iterator device_iterator = this->_devices.begin();  device_iterator != this->_devices.end();  ++device_iterator )
				(*device_iterator)->update(this->get_id());
		};


		void	InputManager::setup()
		{	
			this->register_event(INITIALISE);

			#ifdef	_WIN32
				this->register_event(INIT_KEYBOARD);
				this->register_event(INIT_MOUSE);
				this->register_event(INIT_XBOX360_CONTROLLER);
			#endif
	
			this->register_event(EXIT);
			this->register_event(INPUT_MANAGER_CHANGE_UPDATE_RATE);
			this->register_event(this->_update_rate);
		};

		void	InputManager::on_event( const unsigned long event_code , const Message& parameters )
		{
			if ( event_code == this->_update_rate )
				this->_update();
			else if ( event_code >= INIT_KEYBOARD  &&  event_code <= INIT_XBOX360_CONTROLLER )
				this->_initialise_input_device(event_code,parameters);
			else if ( event_code == INITIALISE )
				#ifdef	_WIN32
					this->_initialise(static_cast<HWND>(parameters.get_pointer(0)));
				#else
					this->_initialise();
				#endif	/* _WIN32 */
			else if ( event_code == EXIT )
				this->_terminate();
			else if ( event_code == INPUT_MANAGER_CHANGE_UPDATE_RATE )
				this->_change_update_rate(static_cast<unsigned long>(parameters.get_integral_data(2)));
		};

	}	/* Core */
}	/* TEXEngine */