#include	"luaShrunkDefaultLibraries.hpp"
#include	"luaconf.h"
#include	<ctype.h>
#include	<ctime>
#include	<cstdio>
#include    <stdio.h>
#include    <stdlib.h>
#include	<sstream>
#include	"..\..\log\logmanager.hpp"

#define		SPACECHARS	" \f\n\r\t\v"



namespace	TEXEngine
{
	namespace	UI
	{
		LuaShrunkDefaultLibraries	LuaShrunkDefaultLibraries::_library;
		luaL_Reg					LuaShrunkDefaultLibraries::_functions[14]	=	{
																						{"assert",LuaShrunkDefaultLibraries::luaB_assert},
																						{"error",LuaShrunkDefaultLibraries::luaB_error},
																						{"getmetatable",LuaShrunkDefaultLibraries::luaB_getmetatable},
																						{"next",LuaShrunkDefaultLibraries::luaB_next},
																						{"print",LuaShrunkDefaultLibraries::luaB_print},
																						{"select",LuaShrunkDefaultLibraries::luaB_select},
																						{"setmetatable",LuaShrunkDefaultLibraries::luaB_setmetatable},
																						{"tonumber",LuaShrunkDefaultLibraries::luaB_tonumber},
																						{"toboolean",LuaShrunkDefaultLibraries::luaB_toboolean},
																						{"tostring",LuaShrunkDefaultLibraries::luaB_tostring},
																						{"type",LuaShrunkDefaultLibraries::luaB_type},
																						{"unpack",LuaShrunkDefaultLibraries::luaB_unpack},
																						{"time",LuaShrunkDefaultLibraries::os_time},
																						{NULL,NULL}
																					};

		Timer						LuaShrunkDefaultLibraries::_timer;




		int	LuaShrunkDefaultLibraries::luaB_print( lua_State* L )
		{
			std::stringstream	buffer;
			int					n = lua_gettop(L);	/* number of arguments */
			


			lua_getglobal(L,"tostring");
			
			for ( int i = 1;  i <= n;  ++i )
			{
				const char*	s = NULL;
				size_t		l = 0;



				lua_pushvalue(L,-1);	/* function to be called */
				lua_pushvalue(L,i);	/* value to print */
				lua_call(L,1,1);

				s = lua_tolstring(L,-1,&l);	/* get result */
				if ( s == NULL )
					return luaL_error(L,LUA_QL("tostring") " must return a string to " LUA_QL("print"));
				
				if ( i>1 )
					buffer << "\t";

				buffer << s;

				lua_pop(L,1);	/* pop result */
			}

			LogManager::get().log_message(buffer.str().c_str());
			
			
			return 0;
		}

		int	LuaShrunkDefaultLibraries::luaB_tonumber( lua_State *L )
		{
			int	base = luaL_optint(L,2,10);



			if ( base == 10 )
			{	/* standard conversion */
				luaL_checkany(L,1);
		
				if ( lua_isnumber(L,1) )
				{
					lua_pushnumber(L,lua_tonumber(L,1));
			
			
					return 1;
				}
			}
			else
			{
				const char*		s1 = luaL_checkstring(L,1);
				char*			s2 = NULL;
				unsigned long	n = 0;
		
		
		
				luaL_argcheck(L,2 <= base  &&  base <= 36,2,"base out of range");
				n = strtoul(s1,&s2,base);

				if ( s1 != s2 )
				{	/* at least one valid digit? */
					while ( isspace((unsigned char)(*s2)) )
						++s2;	/* skip trailing spaces */

					if ( *s2 == '\0' )
					{	/* no invalid trailing characters? */
						lua_pushnumber(L,(lua_Number)n);
				
				
						return 1;
					}
				}
			}
	
			lua_pushnil(L);	/* else not a number */


			return 1;
		}

		
		int	LuaShrunkDefaultLibraries::luaB_toboolean( lua_State* L )
		{
			int	argument_counter = lua_gettop(L);
			int	return_value = 0;


			if ( argument_counter > 1 )
			{
				if ( lua_isboolean(L,1) )
					return_value = lua_toboolean(L,1);
				else if ( lua_isnumber(L,1)  &&  lua_tonumber(L,1) >= 0 )
					return_value = 1;
				else if ( lua_isstring(L,1) )
				{
					lua_getglobal(L,"tonumber");
					lua_pushvalue(L,1);
					lua_pcall(L,1,1,0);

					if ( lua_isnumber(L,-1)  &&  lua_tonumber(L,-1) >= 0 )
						return_value = 1;
				}
			}
			
			lua_pushboolean(L,return_value);

			return 1;
		}

		int	LuaShrunkDefaultLibraries::luaB_error( lua_State *L )
		{
			int	level = luaL_optint(L,2,1);
	
	
	
			lua_settop(L,1);
	
			if ( lua_isstring(L,1)  &&  level > 0 )
			{	/* add extra information? */
				luaL_where(L,level);
				lua_pushvalue(L,1);
				lua_concat(L,2);
			}


			return lua_error(L);
		}

		int	LuaShrunkDefaultLibraries::luaB_getmetatable( lua_State* L )
		{
			luaL_checkany(L,1);

			if ( !lua_getmetatable(L,1) )
			{
				lua_pushnil(L);


				return 1;	/* no metatable */
			}
	
			luaL_getmetafield(L,1,"__metatable");


			return 1;	/* returns either __metatable field (if present) or metatable */
		}

		int	LuaShrunkDefaultLibraries::luaB_setmetatable( lua_State* L )
		{
			int	t = lua_type(L,2);



			luaL_checktype(L,1,LUA_TTABLE);
			luaL_argcheck(L,t == LUA_TNIL  ||  t == LUA_TTABLE,2,"nil or table expected");
	
			if ( luaL_getmetafield(L,1,"__metatable") )
				luaL_error(L,"cannot change a protected metatable");
	
			lua_settop(L,2);
			lua_setmetatable(L,1);


			return 1;
		}

		int	LuaShrunkDefaultLibraries::luaB_type( lua_State* L )
		{
			luaL_checkany(L,1);
			lua_pushstring(L,luaL_typename(L,1));


			return 1;
		}

		int	LuaShrunkDefaultLibraries::luaB_next( lua_State* L )
		{
			luaL_checktype(L,1,LUA_TTABLE);
			lua_settop(L,2);	/* create a 2nd argument if there isn't one */

			if ( lua_next(L,1) )
				return 2;
			else
			{
				lua_pushnil(L);


				return 1;
			}
		}

		int	LuaShrunkDefaultLibraries::luaB_pairs( lua_State* L )
		{
			luaL_checktype(L,1,LUA_TTABLE);
			lua_pushvalue(L,lua_upvalueindex(1));	/* return generator, */
			lua_pushvalue(L,1);	/* state, */
			lua_pushnil(L);	/* and initial value */


			return 3;
		}

		int	LuaShrunkDefaultLibraries::ipairsaux( lua_State* L )
		{
			int	i = luaL_checkint(L,2);



			luaL_checktype(L,1,LUA_TTABLE);
			++i;	/* next value */
			lua_pushinteger(L,i);
			lua_rawgeti(L,1,i);


			return (lua_isnil(L,-1)) ? 0 : 2;
		}

		int	LuaShrunkDefaultLibraries::luaB_ipairs( lua_State* L )
		{
			luaL_checktype(L,1,LUA_TTABLE);
			lua_pushvalue(L,lua_upvalueindex(1));	/* return generator, */
			lua_pushvalue(L,1);	/* state, */
			lua_pushinteger(L,0);	/* and initial value */


			return 3;
		}

		int	LuaShrunkDefaultLibraries::luaB_assert( lua_State* L )
		{
			luaL_checkany(L,1);

			if ( !lua_toboolean(L,1) )
				return luaL_error(L,"%s",luaL_optstring(L,2,"assertion failed!"));


			return lua_gettop(L);
		}

		int	LuaShrunkDefaultLibraries::luaB_unpack( lua_State* L )
		{
			int	i = 0;
			int	e = 0;
			int	n = 0;



			luaL_checktype(L,1,LUA_TTABLE);
			i = luaL_optint(L,2,1);
			e = luaL_opt(L,luaL_checkint,3,luaL_getn(L,1));
	
			if ( i > e)
				return 0;	/* empty range */
	
			n = e - i + 1;	/* number of elements */
	
			if ( n <= 0  ||  !lua_checkstack(L,n) )	/* n <= 0 means arith. overflow */
				return luaL_error(L,"too many results to unpack");

			lua_rawgeti(L,1,i);	/* push arg[i] (avoiding overflow problems) */
			while ( i++ < e )	/* push arg[i + 1...e] */
				lua_rawgeti(L,1,i);


			return n;
		}

		int	LuaShrunkDefaultLibraries::luaB_select( lua_State* L )
		{
			int	n = lua_gettop(L);



			if ( lua_type(L,1) == LUA_TSTRING  &&  *lua_tostring(L,1) == '#' )
			{
				lua_pushinteger(L,n-1);


				return 1;
			}
			else
			{
				int	i = luaL_checkint(L,1);



				if ( i < 0 )
					i = n + i;
				else if ( i > n )
					i = n;
		
				luaL_argcheck(L,1 <= i,1,"index out of range");


				return n - i;
			}
		}

		int	LuaShrunkDefaultLibraries::luaB_tostring( lua_State* L )
		{
			luaL_checkany(L,1);
	
			if ( luaL_callmeta(L,1,"__tostring") )	/* is there a metafield? */
				return 1;	/* use its value */
	
			switch ( lua_type(L,1) )
			{
				case	LUA_TNUMBER:	
										lua_pushstring(L,lua_tostring(L,1));
										break;
				case	LUA_TSTRING:	
										lua_pushvalue(L,1);
										break;
				case	LUA_TBOOLEAN:	
										lua_pushstring(L,(lua_toboolean(L,1) ? "true" : "false"));
										break;
				case	LUA_TNIL:		
										lua_pushliteral(L,"nil");
										break;
				default:				
										lua_pushfstring(L,"%s: %p",luaL_typename(L,1),lua_topointer(L,1));
										break;
			}


			return 1;
		}

		int	LuaShrunkDefaultLibraries::os_time( lua_State *L )
		{
			int		argument_counter = lua_gettop(L);
			double	t = 0;



			if ( argument_counter > 0 )
			{
				if ( lua_isstring(L,1) )
				{
					std::string	argument(lua_tostring(L,1));



					if ( argument == "mlsec" )
						t = LuaShrunkDefaultLibraries::_timer.get_time_in_mlsec();
					else if ( argument == "mcsec" )
						t = LuaShrunkDefaultLibraries::_timer.get_time_in_mcsec();
					else
						t = LuaShrunkDefaultLibraries::_timer.get_time_in_sec();


				}
				else
					t = LuaShrunkDefaultLibraries::_timer.get_time_in_sec();
			}
			else	/* called without args? */
				t = LuaShrunkDefaultLibraries::_timer.get_time_in_sec();

			lua_pushnumber(L,(lua_Number)t);
			
			
			return 1;
		}

		void	LuaShrunkDefaultLibraries::auxopen(lua_State *L , const char* name , lua_CFunction f , lua_CFunction u )
		{
			lua_pushcfunction(L,u);
			lua_pushcclosure(L,f,1);
			lua_setfield(L,-2,name);
		}

		int	LuaShrunkDefaultLibraries::luaB_newproxy( lua_State* L )
		{
			lua_settop(L,1);
			lua_newuserdata(L,0);	/* create proxy */
			if ( lua_toboolean(L,1) == 0 )
				return 1;	/* no metatable */
			else if ( lua_isboolean(L,1) )
			{
				lua_newtable(L);	/* create a new metatable `m' ... */
				lua_pushvalue(L,-1);	/* ... and mark `m' as a valid metatable */
				lua_pushboolean(L,1);
				lua_rawset(L,lua_upvalueindex(1));	/* weaktable[m] = true */
			}
			else
			{
				int	validproxy = 0;	/* to check if weaktable[metatable(u)] == true */
				if ( lua_getmetatable(L,1) )
				{
					lua_rawget(L,lua_upvalueindex(1));
					validproxy = lua_toboolean(L,-1);
					lua_pop(L,1);	/* remove value */
				}
				luaL_argcheck(L,validproxy,1,"boolean or proxy expected");
				lua_getmetatable(L,1);	/* metatable is valid; get it */
			}
			lua_setmetatable(L,2);


			return 1;
		}


		LuaShrunkDefaultLibraries::LuaShrunkDefaultLibraries()	{};
		LuaShrunkDefaultLibraries::~LuaShrunkDefaultLibraries()	{};





		int	LuaShrunkDefaultLibraries::load_library( lua_State* state )
		{
			LuaShrunkDefaultLibraries::_timer.start(); 

			/* set global _G */
			lua_pushvalue(state,LUA_GLOBALSINDEX);
			lua_setglobal(state,"_G");
			/* open lib into global table */
			luaL_register(state,"_G",LuaShrunkDefaultLibraries::_functions);
			lua_pushliteral(state,LUA_VERSION);
			lua_setglobal(state,"_VERSION");	/* set global _VERSION */
			/* `ipairs' and `pairs' need auxliliary functions as upvalues */
			LuaShrunkDefaultLibraries::auxopen(state,"ipairs",LuaShrunkDefaultLibraries::luaB_ipairs,LuaShrunkDefaultLibraries::ipairsaux);
			LuaShrunkDefaultLibraries::auxopen(state,"pairs",LuaShrunkDefaultLibraries::luaB_pairs,LuaShrunkDefaultLibraries::luaB_next);
			/* `newproxy' needs a weaktable as upvalue */
			lua_createtable(state,0,1);	/* new table `w' */
			lua_pushvalue(state,-1);	/* `w' will be its own metatable */
			lua_setmetatable(state,-2);
			lua_pushliteral(state,"kv");
			lua_setfield(state,-2,"__mode");	/* metatable(w).__mode = "kv" */
			lua_pushcclosure(state,LuaShrunkDefaultLibraries::luaB_newproxy,1);
			lua_setglobal(state,"newproxy");	/* set global `newproxy' */


			return 1;
		};

		std::vector<luaL_Reg>	LuaShrunkDefaultLibraries::get_global_functions()
		{
			std::vector<luaL_Reg>	return_value;



			for ( luaL_Reg* pointer = LuaShrunkDefaultLibraries::_functions;  pointer->func;  ++pointer )
				return_value.push_back(*pointer);


			return return_value;
		}





	}	/* UI */
}	/* TEXEngine */