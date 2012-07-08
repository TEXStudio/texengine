#include	<vector>
#include	<string>
#include	"lua.hpp"
#include	"..\uiUtility.hpp"
#include	"..\font.hpp"

#ifndef		TEXENGINE_LUA_UI_LIBRARY_HPP_INCLUDED
	#define	TEXENGINE_LUA_UI_LIBRARY_HPP_INCLUDED



	namespace	TEXEngine
	{
		namespace	UI
		{
			struct	LuaUIFrame
			{
				TEXEngine::UI::ID		_id;
				char					_type[10];
			};

			class	LuaUILibrary
			{
				private:

					static LuaUILibrary				_library;
					static luaL_Reg					_global_functions[4];
					static luaL_Reg					_frame_functions[17];
					static luaL_Reg					_button_functions[2];
					static luaL_Reg					_checkbox_functions[7];
					static luaL_Reg					_container_functions[5];
					static luaL_Reg					_image_functions[5];
					static luaL_Reg					_slider_functions[6];
					static luaL_Reg					_textbox_functions[10];
					static luaL_Reg					_text_component_functions[7];
					static luaL_Reg					_font_functions[5];


					static bool						_check_frame_function_args( lua_State* state , int argc );
					static int						_create_frame( lua_State* state );
					static int						_create_font( lua_State* state );
					static int						_trigger_event( lua_State* state );

					static int						_bind_on_event_function( lua_State* state );
					static int						_set_frame_size( lua_State* state );
					static int						_set_frame_position( lua_State* state );
					static int						_set_background_colour( lua_State* state );
					static int						_set_border_thickness( lua_State* state );
					static int						_set_border_colour( lua_State* state );
					static int						_set_visible( lua_State* state );
					static int						_set_render_border( lua_State* state );
					static int						_set_tab_index( lua_State* state );
					static int						_get_id( lua_State* state );
					static int						_get_frame_size( lua_State* state );
					static int						_get_frame_position( lua_State* state );
					static int						_get_background_colour( lua_State* state );
					static int						_is_visible( lua_State* state );
					static int						_is_enabled( lua_State* state );
					static int						_set_enabled( lua_State* state );


					static int						_button_is_down( lua_State* state );

					static int						_checkbox_set_checked( lua_State* state );
					static int						_checkbox_set_pixel_size( lua_State* state );
					static int						_checkbox_set_cross_colour( lua_State* state );
					static int						_checkbox_is_checked( lua_State* state );
					static int						_checkbox_get_pixel_size( lua_State* state );
					static int						_checkbox_get_cross_colour( lua_State* state );

					static int						_container_attach( lua_State* state );
					static int						_container_detach( lua_State* state );
					static int						_container_set_renderable( lua_State* state );
					static int						_container_set_z_position( lua_State* state );

					static int						_image_set_texu_shift( lua_State* state );
					static int						_image_set_texv_shift( lua_State* state );
					static int						_image_get_texu_shift( lua_State* state );
					static int						_image_get_texv_shift( lua_State* state );

					static int						_slider_set_value( lua_State* state );
					static int						_slider_set_marker_colour( lua_State* state );
					static int						_slider_set_marker_width( lua_State* state );
					static int						_slider_set_marker_padding( lua_State* state );
					static int						_slider_get_value( lua_State* state );

					static int						_textbox_set_blink_delay( lua_State* state );
					static int						_textbox_set_editable( lua_State* state );
					static int						_textbox_set_multiline( lua_State* state );
					static int						_textbox_set_wrapping( lua_State* state );
					static int						_textbox_is_editable( lua_State* state );

					static int						_textbox_set_caret_colour( lua_State* state );
					static int						_textbox_get_caret_colour( lua_State* state );
					static int						_textbox_set_letter_height( lua_State* state );
					static int						_textbox_get_letter_height( lua_State* state );


					static int						_text_component_set_text( lua_State* state );
					static int						_text_component_set_font_id( lua_State* state );
					static int						_text_component_get_text( lua_State* state );
					static int						_text_component_get_font_id( lua_State* state );
					static int						_text_component_set_horiz_align( lua_State* state );
					static int						_text_component_get_horiz_align( lua_State* state );

					static int						_font_set_spacing( lua_State* state );
					static int						_font_get_id( lua_State* state );
					static int						_font_get_grid_size( lua_State* state );
					static int						_font_get_spacing( lua_State* state );


					static std::string				_to_lower( const std::string& input );


					LuaUILibrary();
					~LuaUILibrary();


				public:

					static int						init_library( lua_State* state );
					static int						load_library( lua_State* state );
					static std::vector<luaL_Reg>	get_global_functions();
			};

		}	/* UI */
	}	/* TEXEngine */



#endif		/* TEXENGINE_LUA_UI_LIBRARY_HPP_INCLUDED */