#include	"lua.hpp"
#include	<vector>
#include	"..\..\port\Timer.h"

using TEXEngine::Port::Timer;

#ifndef		TEXENGINE_LUA_SHRUNK_DEFAULT_LIBRARIES_HPP_INCLUDED
	#define	TEXENGINE_LUA_SHRUNK_DEFAULT_LIBRARIES_HPP_INCLUDED



	namespace	TEXEngine
	{
		namespace	UI
		{
			class	LuaShrunkDefaultLibraries
			{
				private:
					
					static LuaShrunkDefaultLibraries	_library;
					static luaL_Reg						_functions[14];
					static Timer						_timer;


					static int							luaB_print( lua_State *L );
					static int							luaB_tonumber( lua_State *L );
					static int							luaB_toboolean( lua_State *L );
					static int							luaB_error( lua_State *L );
					static int							luaB_getmetatable( lua_State *L );
					static int							luaB_setmetatable( lua_State *L );
					static int							luaB_type( lua_State *L );
					static int							luaB_next( lua_State *L );
					static int							luaB_pairs( lua_State *L );
					static int							ipairsaux( lua_State *L );
					static int							luaB_ipairs( lua_State *L );
					static int							luaB_assert( lua_State *L );
					static int							luaB_unpack( lua_State *L );
					static int							luaB_select( lua_State *L );
					static int							luaB_tostring( lua_State *L );
					static int							os_time ( lua_State *L );
					static void							auxopen( lua_State *L , const char *name , lua_CFunction f , lua_CFunction u );
					static int							luaB_newproxy( lua_State *L );

					LuaShrunkDefaultLibraries();
					~LuaShrunkDefaultLibraries();


				public:

					static int							load_library( lua_State* state );
					static std::vector<luaL_Reg>		get_global_functions();
			};
		}	/* UI */
	}	/* TEXEngine */



#endif		/* TEXENGINE_LUA_SHRUNK_DEFAULT_LIBRARIES_HPP_INCLUDED */