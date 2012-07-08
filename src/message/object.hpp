#include	"message.hpp"

#ifndef		TEXENGINE_OBJECT_HPP_INCLUDED
	#define	TEXENGINE_OBJECT_HPP_INCLUDED



	namespace	TEXEngine
	{
		namespace	Core
		{

			/*
				Abstract class used as an interface for all the objects used and created by the game engine.
			*/
			class	Object
			{
				private:

					unsigned long	_id;


				public:

					explicit Object( unsigned long id = 0 );
					virtual ~Object();


					void			set_id( const unsigned long id );
					unsigned long	get_id();


					//	Setup function put any initialisation code ( concerning the message system ) and call once the object has been created
					virtual void	setup();
					//	Event reaction function. It is called by the Message Manager in response to a triggered event
					virtual	void	on_event( const unsigned long event_code , const Message& parameters ) = 0;
					//	Add an event watch for the specific object in the Message Manager
					void			register_event( const unsigned long event_code );
					//	Remove and event watch for the specific object from the Message Manager
					void			unregister_event( const unsigned long event_code );
			};


			/* 
				Function Definitions
			*/

			inline	Object::Object( unsigned long id )	:	_id(id)				{};
			inline	Object::~Object()											{};


			inline void				Object::setup()								{};
			inline void				Object::set_id( const unsigned long id )	{ this->_id = id; };
			inline unsigned long	Object::get_id()							{ return this->_id; };

		}	/* Core */
	}	/* TEXEngine */



#endif		/* TEXENGINE_OBJECT_HPP_INCLUDED */