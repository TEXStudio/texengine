#include	"luaState.hpp"
#include	<cstdlib>
#include	<stdlib.h>
#include	<sstream>
#include <string>
#include <fstream>



namespace	TEXEngine
{
	namespace	UI
	{

		void*									LuaState::_ud = NULL;
		lua_CFunction							LuaState::_lua_user_error_function = NULL;


		void*	LuaState::_lua_alloc( void* , void* ptr , size_t , size_t nsize )
		{
			void*	return_value;



			if ( nsize == 0 )
			{
				if ( ptr != NULL )
					free(ptr);

				return_value = NULL;
			}
			else
				return_value = realloc(ptr,nsize);


			return return_value;
		};

		std::string	LuaState::_lua_stack_dump( lua_State* state )
		{
			int					top = lua_gettop(state);
			std::stringstream	buffer;
			std::string			return_value;				
	
	
	
			for ( int i = 1;  i <= top;  ++i )
			{  /* repeat for each level */
				int	t = lua_type(state, i);
		
		
		
				switch ( t )
				{
					case LUA_TSTRING:	/* strings */
										buffer	<< lua_tostring(state, i);
										break;
			
					case LUA_TBOOLEAN:	/* booleans */
										buffer	<< (lua_toboolean(state, i) ? "true" : "false");
										break;
			
					case LUA_TNUMBER:	/* numbers */
										buffer	<< lua_tonumber(state, i);
										break;
	
					default:			/* other values */
										buffer	<< lua_typename(state, t);
										break;
				}
		
				buffer << std::endl;  /* put a separator */
			}
	
			buffer	<< std::endl;  /* end the listing */
			while( !buffer.eof() )
			{
				std::string	temp;



				std::getline(buffer,temp);
				temp += "\n";
				return_value += temp;
			}


			return return_value;
		};

		void	LuaState::_push_library( lua_State* state , std::string name , lua_CFunction function )
		{
			lua_pushcfunction(state,function);
			lua_pushstring(state,name.c_str());
			lua_pcall(state,1,0,0);
		};

		
		void	LuaState::set_error_function( lua_CFunction function )
		{
			LuaState::_lua_user_error_function = function;
		};


		LuaState::LuaState()	:	
			_state(NULL)
		{
		};

		LuaState::~LuaState()
		{
			this->destroy();
		};


		std::string	LuaState::get_stack()
		{
			return LuaState::_lua_stack_dump(this->_state);
		};


		int	LuaState::create()
		{
			int	return_value = 1;



			if ( this->_state == NULL )
			{
				this->_state = lua_newstate(LuaState::_lua_alloc,LuaState::_ud);
	
				if ( this->_state == NULL )
						return_value = 0;
				else
				{
					if ( LuaState::_lua_user_error_function != NULL )
						lua_atpanic(this->_state,LuaState::_lua_user_error_function);
				}
			}
	

			return return_value;
		};

		void	LuaState::destroy()
		{
			if ( this->_state != NULL )
			{
				lua_close(this->_state);
				this->_state = NULL;
			}
		};


		void	LuaState::load_lib_base()		{ this->_push_library(this->_state,std::string(""),luaopen_base); };
		void	LuaState::load_lib_package()	{ this->_push_library(this->_state,std::string(LUA_LOADLIBNAME),luaopen_package); };
		void	LuaState::load_lib_table()		{ this->_push_library(this->_state,std::string(LUA_TABLIBNAME),luaopen_table); };
		void	LuaState::load_lib_IO()			{ this->_push_library(this->_state,std::string(LUA_IOLIBNAME),luaopen_io); };
		void	LuaState::load_lib_OS()			{ this->_push_library(this->_state,std::string(LUA_OSLIBNAME),luaopen_os); };
		void	LuaState::load_lib_string()		{ this->_push_library(this->_state,std::string(LUA_STRLIBNAME),luaopen_string); };
		void	LuaState::load_lib_math()		{ this->_push_library(this->_state,std::string(LUA_MATHLIBNAME),luaopen_math); };
		void	LuaState::load_lib_debug()		{ this->_push_library(this->_state,std::string(LUA_DBLIBNAME),luaopen_debug); };
		void	LuaState::load_all_libs()
		{
			if ( this->_state )
				luaL_openlibs(this->_state);
		};

		void	LuaState::load_library( const luaL_Reg& library )
		{
			std::string	name(library.name);


			this->load_library(name,library.func);
		};

		void	LuaState::load_library( const std::string& name , lua_CFunction library )
		{
			if ( this->_state )
				this->_push_library(this->_state,name,library);
		};

		void	LuaState::load_variable( const std::string& name)
		{
			if ( this->_state )
			{
				lua_pushnil(this->_state);
				lua_setglobal(this->_state,name.c_str());
			}
		};

		void	LuaState::load_variable( const std::string& name , bool value )
		{
			if ( this->_state )
			{
				lua_pushboolean(this->_state,( value  ?  1 : 0 ) );
				lua_setglobal(this->_state,name.c_str());
			}
		};

		void	LuaState::load_variable( const std::string& name , int value )
		{
			if ( this->_state )
			{
				lua_pushinteger(this->_state,static_cast<lua_Integer>(value));
				lua_setglobal(this->_state,name.c_str());
			}
		};

		void	LuaState::load_variable( const std::string& name,  unsigned int value )
		{
			if ( this->_state )
			{
				lua_pushinteger(this->_state,static_cast<lua_Integer>(value));
				lua_setglobal(this->_state,name.c_str());
			}
		};
		
		void	LuaState::load_variable( const std::string& name , double value )
		{
			if ( this->_state )
			{
				lua_pushnumber(this->_state,static_cast<lua_Number>(value));
				lua_setglobal(this->_state,name.c_str());
			}
		};

		void	LuaState::load_variable( const std::string& name , const std::string& value )
		{
			if ( this->_state )
			{
				lua_pushstring(this->_state,value.c_str());
				lua_setglobal(this->_state,name.c_str());
			}
		};

		void	LuaState::load_variable( const luaL_Reg& function )
		{
			std::string	name(function.name);



			this->load_variable(name,function.func);
		};

		void	LuaState::load_variable( const std::string& name , lua_CFunction function )
		{
			if ( this->_state )
				lua_register(this->_state,name.c_str(),function);
		}


		int	LuaState::load_script_file( std::string filename )
		{
			int	return_value = LUA_ERRFILE;



			if ( filename != "" )
			{
				#ifdef _WIN32
					return_value = luaL_loadfile(this->_state,filename.c_str());				
				#endif
			}


			return return_value;
		};

		int	LuaState::run_script()
		{
			int	return_value = LUA_ERRFILE;



			if ( this->_state )
				return_value = lua_pcall(this->_state,0,LUA_MULTRET,0);


			return return_value;
		};

		int	LuaState::run_lua_function( lua_CFunction function )
		{
			int	return_value = LUA_ERRRUN;



			if ( this->_state )
			{
				lua_pushcfunction(this->_state,function);
				return_value = lua_pcall(this->_state,0,0,0);
			}
			

			return return_value;
		};

		int	LuaState::run_function( int (*function)(lua_State*)  )
		{
			int	return_value = LUA_ERRRUN;



			if ( function )
			{
				if ( this->_state )
					return_value = function(this->_state);
			}


			return	return_value;
		};
	}	/* UI */
}	/* TEXEngine */