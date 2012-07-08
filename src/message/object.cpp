#include	"Object.hpp"
#include	"TEXEngineInterface.hpp"



namespace	TEXEngine
{
	namespace	Core
	{

		void	Object::register_event( const unsigned long event_code )	{ add_event_watch(event_code,this); };
		void	Object::unregister_event( const unsigned long event_code)	{ remove_event_watch(event_code,this); };

	}	/* Core */
}	/* TEXEngine */