#ifndef		TEXENGINE_INPUTDEVICE_HPP_INCLUDED
	#define	TEXENGINE_INPUTDEVICE_HPP_INCLUDED



namespace	TEXEngine
{
	namespace	Device
	{
		
		// Abstract class used by the input manager and an interface to handle input devices
		class	InputDevice
		{
			public:
				InputDevice()			{};
				virtual ~InputDevice()	{};

				//	Function used to initialise device content
				virtual void	initialise() = 0;
				//	Function used to update the status of the device
				virtual void	update( const unsigned long id ) = 0;
				//	Function used to terminate any device content
				virtual void	terminate() = 0;
		};

	}	/* namespace Device */
}	/* namespace TEXEngine */



#endif		/* TEXENGINE_INPUTDEVICE_HPP_INCLUDED */