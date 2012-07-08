#include	"messageManager.hpp"
#include	"TEXEngineInterface.hpp"



namespace	TEXEngine
{
	namespace	Core
	{

		MessageManager	MessageManager::_manager;


		void	MessageManager::_add_event_to_event_map( const unsigned long event_code , Object* entity )
		{
			this->_event_map.insert(std::pair<unsigned long,std::vector<Object*> >(event_code,std::vector<Object*>(1,entity)));
		};

		void	MessageManager::_remove_event_from_event_map( std::map<unsigned long,std::vector<Object*> >::iterator event_iterator )
		{
			this->_event_map.erase(event_iterator);
		};

		void	MessageManager::_add_object_to_event_map( const unsigned long event_code , Object* entity )
		{
			std::map<unsigned long,std::vector<Object*> >::iterator	event_iterator = this->_event_map.find(event_code);



			if ( event_iterator != this->_event_map.end() )
			{
				std::vector<Object*>::iterator	event_object_iterator = (event_iterator->second).begin();



				while( event_object_iterator != (event_iterator->second).end() )
				{
					if ( (*event_object_iterator) == entity )
						break;
					else
						++event_object_iterator;
				};

				if ( event_object_iterator == (event_iterator->second).end() )
				{
					(event_iterator->second).push_back(entity);
					this->_increase_object_events_in_object_map(entity);
				}
			}
			else
			{
				this->_add_event_to_event_map(event_code,entity);
				this->_increase_object_events_in_object_map(entity);
			}
		};

		void	MessageManager::_remove_object_from_event_map( const unsigned long event_code , Object* entity )
		{
			this->_remove_object_from_event_map(this->_event_map.find(event_code),entity);
		};

		void	MessageManager::_remove_object_from_event_map( std::map<unsigned long,std::vector<Object*> >::iterator event_iterator , Object* entity )
		{
			if ( event_iterator != this->_event_map.end() )
			{
				std::vector<Object*>::iterator	event_object_iterator = (event_iterator->second).begin();



				while( event_object_iterator != (event_iterator->second).end() )
				{
					if ( (*event_object_iterator) == entity )
					{
						(event_iterator->second).erase(event_object_iterator);
						this->_decrease_object_events_in_object_map(entity);

						if ( (event_iterator->second).size() == 0  &&  !this->_keep_history )
							this->_remove_event_from_event_map(event_iterator);

						break;
					}
					else
						++event_object_iterator;
				};
			}
		};

		void	MessageManager::_add_object_to_object_map( Object* entity , const unsigned long starting_count )
		{
			this->_object_map.insert(std::pair<Object*,unsigned long>(entity,starting_count));
		};


		void	MessageManager::_remove_object_from_object_map( std::map<Object*,unsigned long>::iterator object_iterator )
		{
			this->_object_map.erase(object_iterator);
		};

		void	MessageManager::_increase_object_events_in_object_map( Object* entity )
		{
			std::map<Object*,unsigned long>::iterator	object_iterator = this->_object_map.find(entity);



			if ( object_iterator != this->_object_map.end() )
				++(object_iterator->second);
			else
				this->_add_object_to_object_map(entity,1);
		};

		void	MessageManager::_decrease_object_events_in_object_map( Object* entity )
		{
			std::map<Object*,unsigned long>::iterator	object_iterator = this->_object_map.find(entity);



			if ( object_iterator != this->_object_map.end() )
			{
				if ( object_iterator->second > 0 )
					--(object_iterator->second);
	
				if ( object_iterator->second == 0  &&  !this->_keep_history )
					this->_remove_object_from_object_map(object_iterator);
			}
		};

		void	MessageManager::_purge_history()
		{
			std::map<Object*,unsigned long>::iterator				object_iterator = this->_object_map.begin();
			std::map<unsigned long,std::vector<Object*> >::iterator	event_iterator = this->_event_map.begin();



			while( object_iterator != this->_object_map.end() )
			{
				if ( object_iterator->second == 0 )
					this->_remove_object_from_object_map(object_iterator++);
				else
					++object_iterator;
			}

			while( event_iterator != this->_event_map.end() )
			{
				if ( (event_iterator->second).size() == 0 )
					this->_remove_event_from_event_map(event_iterator++);
				else
					++event_iterator;
			}
		};

		void	MessageManager::_delete_object( Object* entity )
		{
			if ( entity != NULL )
			{
				std::map<Object*,unsigned long>::iterator	object_iterator = this->_object_map.find(entity);



				if ( object_iterator != this->_object_map.end() )
				{
					for ( std::map<unsigned long,std::vector<Object*> >::iterator event_iterator = this->_event_map.begin();  event_iterator != this->_event_map.end();  ++event_iterator )
						this->_remove_object_from_event_map(event_iterator,entity);

					this->_remove_object_from_object_map(object_iterator);
				}
			}
		};

		void	MessageManager::_add_event_to_all_objects_event_list( const unsigned long event_code )
		{
			std::vector<unsigned long>::iterator	all_object_event_iterator = this->_all_objects_event_list.begin();
	


			while ( all_object_event_iterator != this->_all_objects_event_list.end() )
			{
				if ( (*all_object_event_iterator) == event_code )
					break;
				else
					++all_object_event_iterator;
			}

			if ( all_object_event_iterator == this->_all_objects_event_list.end() )
				this->_all_objects_event_list.push_back(event_code);
		};

		void	MessageManager::_remove_event_from_all_objects_event_list( const unsigned long event_code )
		{
			std::vector<unsigned long>::iterator	all_object_event_iterator = this->_all_objects_event_list.begin();
	


			while ( all_object_event_iterator != this->_all_objects_event_list.end() )
			{
				if ( (*all_object_event_iterator) == event_code )
				{
					this->_all_objects_event_list.erase(all_object_event_iterator);
					break;
				}
				else
					++all_object_event_iterator;
			}
		};

		bool	MessageManager::_parse_manager_events( const unsigned long event_code , const Message& parameters )
		{
			bool	return_value = false;



			if ( event_code == MESSAGE_MANAGER_KEEP_HISTORY )
			{
				this->_keep_history = ( parameters.get_integral_data(2) > 0  ?  true : false );

				if ( !this->_keep_history )
					this->_purge_history();

				return_value = true;
			}
			else if ( event_code == MESSAGE_MANAGER_OBJECT_CREATED )
			{
				Object*	entity = static_cast<Object*>(parameters.get_pointer(0));



				if ( entity != NULL )
					this->_add_object_to_object_map(entity,0);

				return_value = true;
			}
			else if ( event_code == MESSAGE_MANAGER_OBJECT_DELETED )
			{
				this->_delete_object(static_cast<Object*>(parameters.get_pointer(0)));
				return_value = true;
			}
			else if ( event_code == ALL_EVENTS )
			{
				return_value = true;
			}


			return return_value;
		};


		void	MessageManager::on_event( const unsigned long event_code , const Message& parameters )
		{
			if ( !this->_parse_manager_events(event_code,parameters) )
			{
				std::vector<unsigned long>::iterator	all_objects_event_iterator = this->_all_objects_event_list.begin();



				while ( all_objects_event_iterator != this->_all_objects_event_list.end() )
				{
					if ( (*all_objects_event_iterator) == event_code )
						break;
					else
						++all_objects_event_iterator;
				}

				if ( all_objects_event_iterator == this->_all_objects_event_list.end() )
				{
					std::map<unsigned long,std::vector<Object*> >::iterator	all_events_iterator = this->_event_map.find(ALL_EVENTS);
					std::map<unsigned long,std::vector<Object*> >::iterator event_iterator = this->_event_map.find(event_code);



					if ( all_events_iterator != this->_event_map.end() )
					{
						std::vector<Object*>::iterator	all_events_object_iterator = (all_events_iterator->second).begin();
						int								size = all_events_iterator->second.size();
						int								pos = 0;



						while( pos < size )
						{
							all_events_iterator = this->_event_map.find(ALL_EVENTS);

							if ( all_events_iterator != this->_event_map.end() )
							{
								int	i = 0;



								all_events_object_iterator = (all_events_iterator->second).begin();

								while ( i < pos  &&  all_events_object_iterator != all_events_iterator->second.end() )
								{
									++all_events_object_iterator;
									++i;
								}

								if ( all_events_object_iterator != all_events_iterator->second.end() )
									(*all_events_object_iterator)->on_event(event_code,parameters);
							

								++pos;
							}
							else
								break;

						}
					}

					if ( event_iterator != this->_event_map.end() )
					{
						std::vector<Object*>::iterator	event_object_iterator = (event_iterator->second).begin();
						int								size = event_iterator->second.size();
						int								pos = 0;



						while( pos < size )
						{
							event_iterator = this->_event_map.find(event_code);
							
							if ( event_iterator != this->_event_map.end() )
							{
								int	i = 0;



								event_object_iterator = event_iterator->second.begin();

								while( i < pos  &&  event_object_iterator != event_iterator->second.end() )
								{
									++event_object_iterator;
									++i;
								}

								if ( event_object_iterator != event_iterator->second.end() )
									(*event_object_iterator)->on_event(event_code,parameters);
							

								++pos;
							}
							else
								break;
						}
					}
				}
				else
				{
					std::map<Object*,unsigned long>::iterator object_map_iterator = this->_object_map.begin();



					while ( object_map_iterator != this->_object_map.end() )
						object_map_iterator->first->on_event(event_code,parameters);
				}
			}
		};

		void	MessageManager::on_object_event( const unsigned long event_code , const Message& parameters )
		{
			std::map<unsigned long,std::vector<Object*> >::iterator	event_iterator = this->_event_map.find(event_code);



			if ( event_iterator != this->_event_map.end() )
			{
				std::vector<Object*>::iterator	event_object_iterator = (event_iterator->second).begin();



				while ( event_object_iterator != (event_iterator->second).end() )
				{
					if ( (*event_object_iterator)->get_id() == parameters.get_integral_data(1) )
					{
						(*event_object_iterator)->on_event(event_code,parameters);
						break;
					}
					else
						++event_object_iterator;
				}
			}
		};

		void	MessageManager::register_event( const unsigned long event_code , Object* entity )
		{
			if ( entity != NULL )
				this->_add_object_to_event_map(event_code,entity);
			else
				this->_add_event_to_all_objects_event_list(event_code);
		};

		void	MessageManager::unregister_event( const unsigned long event_code , Object* entity )
		{
			if ( entity != NULL )
				this->_remove_object_from_event_map(event_code,entity);
			else
				this->_remove_event_from_all_objects_event_list(event_code);
		};

	}	/* Core */
}	/* TEXEngine */