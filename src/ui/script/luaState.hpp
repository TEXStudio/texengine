#pragma	once

#include	"lua.hpp"
#include	<string>
#include	<vector>
#include	<map>

#ifndef		TEXENGINE_LUA_STATE_HPP_INCLUDED
	#define	TEXENGINE_LUA_STATE_HPP_INCLUDED



	namespace	TEXEngine
	{
		namespace	UI
		{

			class	LuaState
			{

				private:	
			
					static void*			_ud;
					static lua_CFunction	_lua_user_error_function;
					
					lua_State*				_state;


					static void*			_lua_alloc( void* ud , void* ptr , size_t osize , size_t nsize );
					static std::string		_lua_stack_dump( lua_State* state );
					static void				_push_library( lua_State* state , std::string name , lua_CFunction function );


				public:

					static void				set_error_function( lua_CFunction function );


					LuaState();
					~LuaState();


					std::string				get_stack();


					int						create();
					void					destroy();


					void					load_lib_base();
					void					load_lib_table();
					void					load_lib_IO();
					void					load_lib_OS();
					void					load_lib_string();
					void					load_lib_math();
					void					load_lib_debug();
					void					load_lib_package();
					void					load_all_libs();
					void					load_library( const luaL_Reg& library );
					void					load_library( const std::string& name , lua_CFunction function );
					void					load_variable( const std::string& name);
					void					load_variable( const std::string& name , bool value );
					void					load_variable( const std::string& name , int value );
					void					load_variable( const std::string& name,  unsigned int value );
					void					load_variable( const std::string& name , double value );
					void					load_variable( const std::string& name , const std::string& value );
					void					load_variable( const luaL_Reg& function );
					void					load_variable( const std::string& name , lua_CFunction function );


					int						load_script_file( const std::string filename );
					int						run_script();
					int						run_lua_function( const lua_CFunction function );
					int						run_function( int (*function)(lua_State*)  );
			};

		}	/* UI */
	}	/* TEXEngine */



#endif		/* _LUA_STATE_HPP_INCLUDED */