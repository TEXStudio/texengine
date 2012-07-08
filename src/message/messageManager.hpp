#include	<map>
#include	<vector>
#include	"message.hpp"
#include	"object.hpp"

#ifndef		TEXENGINE_MESSAGE_MANAGER_HPP_INCLUDED
	#define	TEXENGINE_MESSAGE_MANAGER_HPP_INCLUDED



	namespace	TEXEngine
	{
		namespace	Core
		{

			/*
				Class responsible for handling any tiggered events, and distributing the messages attached to each event
				to the Objects interested in those events.
			*/
			class	MessageManager
			{
				private:

					//	Single instance of the MessageManager class
					static MessageManager							_manager;
					//	A map containing any registered events and the Objects interested in those events.
					std::map<unsigned long,std::vector<Object*> >	_event_map;
					//	A map containing the list of the Objects with a registered event, and how many events are assosiated with this Object.
					std::map<Object*,unsigned long>					_object_map;
					std::vector<unsigned long>						_all_objects_event_list;
					bool											_keep_history;


					MessageManager();
					~MessageManager();								

					//	Adds an event to the event map. It also adds the given object to the array assosiated with that event.
					void											_add_event_to_event_map( const unsigned long event_code , Object* entity );
					//	Removes an event from the event map.
					void											_remove_event_from_event_map( std::map<unsigned long,std::vector<Object*> >::iterator event_iterator );
					//	Adds an object to the array assosiated with the given event.
					void											_add_object_to_event_map( const unsigned long event_code , Object* entity );
					//	Removes an object from the array assosiated with the given event.
					void											_remove_object_from_event_map( const unsigned long event_code , Object* entity );
					void											_remove_object_from_event_map( std::map<unsigned long,std::vector<Object*> >::iterator event_iterator , Object* entity );
					//	Adds an object to the object map and initialises the number of events assosiated with to 0.
					void											_add_object_to_object_map( Object* object ,  const unsigned long starting_count = 0 );
					//	Removes an object from the object map.
					void											_remove_object_from_object_map( std::map<Object*,unsigned long>::iterator object_iterator );
					//	Increases the number of events that the object is interested in.
					void											_increase_object_events_in_object_map( Object* entity );
					//	Decreases the number of events that the object is interested in.
					void											_decrease_object_events_in_object_map( Object* entity );
					//	Removes any events and objects that are not currently assosiated with any objects and events respectively.
					void											_purge_history();
					//	Removes and object both from the event_map and the  the object map from the object list given, adding and removing objects where necessary.
					void											_delete_object( Object* entity );
					//	Add the given event to the notify all objects list.
					void											_add_event_to_all_objects_event_list( const unsigned long event_code );
					//	Remove the gineve event from the notify all object list.
					void											_remove_event_from_all_objects_event_list( const unsigned long event_code );
					// function for parsing the events relevant to the event_manager.
					bool											_parse_manager_events( const unsigned long event_code , const Message& parameters );


				public:

					//	Function responsible for making available the single instance of this singleton class.
					static MessageManager&							get();


					//	Function handling an event when the global trigger_event function is called. It parses the message and relays it to any interested object.
					void											on_event( const unsigned long event_code , const Message& parameters );
					//	Function handling an event when the global trigger_object_event function is called. It parses the message and relays it to the specified object.
					void											on_object_event( const unsigned long event_code , const Message& parameters );
					//	Function handling the AddEventWatch global function. It registers a Object for event notification.
					void											register_event( const unsigned long event_code , Object* entity );
					//	Function handling the RemoveEventWatch global function. It removes an event notification for the given Object.
					void											unregister_event( const unsigned long event_code , Object* entity );
			};



			/*
				Function implementations
			*/

			inline	MessageManager::MessageManager()	:
				_event_map() , _object_map() , _all_objects_event_list(0,0) , _keep_history(true)	{};
			inline	MessageManager::~MessageManager()												{};


			inline MessageManager&	MessageManager::get()											{ return _manager; };

		}	/* Core */
	};	/* TEXEngine */



#endif		/* TEXENGINE_MESSAGE_MANAGER_HPP_INCLUDED */