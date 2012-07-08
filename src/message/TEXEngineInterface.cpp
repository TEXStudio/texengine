#include	"TEXEngineInterface.hpp"
#include	"messageManager.hpp"
#include	"..\renderer\renderManager.hpp"
#include	"..\input\inputManager.hpp"
#ifdef _WIN32
#include	"..\audio\audioManager.hpp"
#endif //_WIN32
//#include	"..\physics\PhysicsManager.h"
#include	"..\ui\uiManager.hpp"
/*#include	"..\asset\assetManager.hpp"
#include	"..\asset\shaderManager.hpp"
#include	"..\scene\sceneManager.hpp"*/
#include	"..\log\logmanager.hpp"



namespace TEXEngine {
	namespace Core {

		void	setup()
		{
			RenderManager::get().setup();
			InputManager::get().setup();
#ifdef _WIN32
			AudioManager::get().setup();
#endif //_WIN32
			//PhysicsManager::get().setup();
			UIManager::get().setup();
			//AssetManager::get().setup();
			ShaderManager::get().setup();
			//SceneManager::get().setup();
			LogManager::get().setup();
		};

		void	trigger_event( const unsigned long event_code , const Message& parameters )			{ MessageManager::get().on_event(event_code,parameters); };
		void	trigger_object_event( const unsigned long event_code , const Message& parameters )	{ MessageManager::get().on_object_event(event_code,parameters); };
		void	global_register_event( const unsigned long event_code )							{ MessageManager::get().register_event(event_code,NULL); };
		void	global_unregister_event( const unsigned long event_code )						{ MessageManager::get().unregister_event(event_code,NULL); };
		void	add_event_watch( const unsigned long event_code , Object* entity )				{ MessageManager::get().register_event(event_code,entity); };
		void	remove_event_watch( const unsigned long event_code , Object* entity )			{ MessageManager::get().unregister_event(event_code,entity); };

	} /* namespace Core */
} /* namespace TEXEngine */