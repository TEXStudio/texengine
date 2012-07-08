#include	"luaUILibrary.hpp"
#include	<string>
#include	<sstream>
#include	<cstring>
#include	"..\uiManager.hpp"
#include	"..\button.hpp"
#include	"..\checkbox.hpp"
#include	"..\container.hpp"
#include	"..\font.hpp"
#include	"..\uiImage.hpp"
#include	"..\label.hpp"
#include	"..\slider.hpp"
#include	"..\textbox.hpp"
#include	"..\textComponent.hpp"
#include	"..\..\util\color.hpp"
#include	"..\..\message\TEXEngineInterface.hpp"



namespace	TEXEngine
{
	namespace	UI
	{

		LuaUILibrary	LuaUILibrary::_library;
		luaL_Reg		LuaUILibrary::_global_functions[4]		=	{
																		{"CreateFrame",LuaUILibrary::_create_frame},
																		{"CreateFont",LuaUILibrary::_create_font},
																		{"TriggerEvent",LuaUILibrary::_trigger_event},
																		{NULL,NULL}
																	};
		luaL_Reg		LuaUILibrary::_frame_functions[17]		=	{
																		{"EventFunction",LuaUILibrary::_bind_on_event_function},
																		{"SetSize",LuaUILibrary::_set_frame_size},
																		{"SetPosition",LuaUILibrary::_set_frame_position},
																		{"SetBackgroundColour",LuaUILibrary::_set_background_colour},
																		{"SetBorderThickness",LuaUILibrary::_set_border_thickness},
																		{"SetBorderColour",LuaUILibrary::_set_border_colour},
																		{"Visible",LuaUILibrary::_set_visible},
																		{"RenderBorder",LuaUILibrary::_set_render_border},
																		{"TabIndex",LuaUILibrary::_set_tab_index},
																		{"GetId",LuaUILibrary::_get_id},
																		{"GetSize",LuaUILibrary::_get_frame_size},
																		{"GetPosition",LuaUILibrary::_get_frame_position},
																		{"GetBackgroundColour",LuaUILibrary::_get_background_colour},
																		{"IsVisible",LuaUILibrary::_is_visible},
																		{"IsEnabled",LuaUILibrary::_is_enabled},
																		{"Enabled",LuaUILibrary::_set_enabled},
																		{NULL,NULL}
																	};
		luaL_Reg	LuaUILibrary::_button_functions[2]			=	{
																		{"IsDown",LuaUILibrary::_button_is_down},
																		{NULL,NULL}
																	};
		luaL_Reg	LuaUILibrary::_checkbox_functions[7]		=	{
																		{"SetChecked",LuaUILibrary::_checkbox_set_checked},
																		{"SetPixelSize",LuaUILibrary::_checkbox_set_pixel_size},
																		{"SetCrossColour",LuaUILibrary::_checkbox_set_cross_colour},
																		{"IsChecked",LuaUILibrary::_checkbox_is_checked},
																		{"GetPixelSize",LuaUILibrary::_checkbox_get_pixel_size},
																		{"GetCrossColour",LuaUILibrary::_checkbox_get_cross_colour},
																		{NULL,NULL}
																	};
		luaL_Reg	LuaUILibrary::_container_functions[5]		=	{
																		{"Attach",LuaUILibrary::_container_attach},
																		{"Detach",LuaUILibrary::_container_detach},
																		{"Renderable",LuaUILibrary::_container_set_renderable},
																		{"SetZ",LuaUILibrary::_container_set_z_position},
																		{NULL,NULL}
																	};
		luaL_Reg	LuaUILibrary::_image_functions[5]			=	{
																		{"SetUShift",LuaUILibrary::_image_set_texu_shift},
																		{"SetVShift",LuaUILibrary::_image_set_texv_shift},
																		{"GetUShift",LuaUILibrary::_image_get_texu_shift},
																		{"GetVShift",LuaUILibrary::_image_get_texv_shift},
																		{NULL,NULL}
																	};
		luaL_Reg	LuaUILibrary::_slider_functions[6]			=	{
																		{"SetValue",LuaUILibrary::_slider_set_value},
																		{"SetMarkerColour",LuaUILibrary::_slider_set_marker_colour},
																		{"SetMarkerWidth",LuaUILibrary::_slider_set_marker_width},
																		{"SetMarkerPadding",LuaUILibrary::_slider_set_marker_padding},
																		{"GetValue",LuaUILibrary::_slider_get_value},
																		{NULL,NULL}
																	};
		luaL_Reg	LuaUILibrary::_textbox_functions[10]			=	{
																		{"SetBlinkDelay",LuaUILibrary::_textbox_set_blink_delay},
																		{"Editable",LuaUILibrary::_textbox_set_editable},
																		{"Multiline",LuaUILibrary::_textbox_set_multiline},
																		{"Wrapping",LuaUILibrary::_textbox_set_wrapping},
																		{"IsEdittable",LuaUILibrary::_textbox_is_editable},
																		{"SetCaretColour",LuaUILibrary::_textbox_set_caret_colour},
																		{"GetCaretColour",LuaUILibrary::_textbox_get_caret_colour},
																		{"SetLetterHeight",LuaUILibrary::_textbox_set_letter_height},
																		{"GetLetterHeight",LuaUILibrary::_textbox_get_letter_height},
																		{NULL,NULL}
																	};
		luaL_Reg	LuaUILibrary::_text_component_functions[7]	=	{
																		{"SetText",LuaUILibrary::_text_component_set_text},
																		{"SetFont",LuaUILibrary::_text_component_set_font_id},
																		{"GetText",LuaUILibrary::_text_component_get_text},
																		{"GetFont",LuaUILibrary::_text_component_get_font_id},
																		{"SetHorizAlign",LuaUILibrary::_text_component_set_horiz_align},
																		{"GetHorizAlign",LuaUILibrary::_text_component_get_horiz_align},
																		{NULL,NULL}
																	};
		luaL_Reg	LuaUILibrary::_font_functions[5]			=	{
																		{"SetSpacing",LuaUILibrary::_font_set_spacing},
																		{"GetId",LuaUILibrary::_font_get_id},
																		{"GetGridSize",LuaUILibrary::_font_get_grid_size},
																		{"GetSpacing",LuaUILibrary::_font_get_spacing},
																		{NULL,NULL}
																	};
		
		bool	LuaUILibrary::_check_frame_function_args( lua_State* state , int argc )
		{
			bool						valid = false;
			std::vector<std::string>	tables(9,"");



			
			tables[0] = "UI._frame_metatable";
			tables[1] = "UI._button_metatable";
			tables[2] = "UI._checkbox_metatable";
			tables[3] = "UI._container_metatable";
			tables[4] = "UI._image_metatable";
			tables[5] = "UI._label_metatable";
			tables[6] = "UI._slider_metatable";
			tables[7] = "UI._textbox_metatable";
			tables[8] = "UI._font_metatable";


			if ( lua_isuserdata(state,argc) )
			{
				if ( lua_getmetatable(state,argc) )
				{
					for ( std::vector<std::string>::iterator it = tables.begin();  it != tables.end();  ++it )
					{
						lua_getfield(state,LUA_REGISTRYINDEX,(*it).c_str());

						if ( lua_rawequal(state,-1,-2) )
						{
							valid = true;
							lua_pop(state,1);
							break;
						}

						lua_pop(state,1);
					};

					lua_pop(state,1);
				}
			}

			luaL_argcheck(state,valid,argc,"A UI frame was expected!");
			

			return valid;
		};


		int	LuaUILibrary::_create_frame( lua_State* state )
		{
			int	argument_counter	= lua_gettop(state);



			if ( argument_counter >= 3 )
			{
				std::string			frame_type = "container";
				std::string			frame_name = "";
				TEXEngine::UI::ID	id = 0;



				luaL_argcheck(state,lua_isstring(state,1),1,"Type argument must be a string!");
				luaL_argcheck(state,lua_isstring(state,2)  ||  lua_isnil(state,2),2,"Name argument must be a string or nil!");
				luaL_argcheck(state,lua_isnumber(state,3),3,"ID argument must be a number!");

				frame_type = lua_tostring(state,1);
				frame_type = LuaUILibrary::_to_lower(frame_type);
				frame_type.resize(sizeof(LuaUIFrame()._type)-1,'\0');

				if ( lua_isstring(state,2) )
					frame_name += lua_tostring(state,2);
							
				id = static_cast<TEXEngine::UI::ID>(lua_tointeger(state,3));

				if ( id >= TEXEngine::Core::UIManager::MIN_ID )
				{
					std::string	type(frame_type.c_str());
					LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_newuserdata(state,sizeof(LuaUIFrame)));



					if ( frame == NULL )
						luaL_error(state,"Frame creation failed due to memory allocation problems");


					if ( type == "button" )
					{
						TEXEngine::Core::UIManager::get().create_button(id);
						luaL_getmetatable(state,"UI._button_metatable");
						lua_setmetatable(state,-2);
						frame->_id = id;
						memcpy(frame->_type,frame_type.c_str(),sizeof(frame->_type));
					}
					else if ( type == "checkbox" )
					{
						TEXEngine::Core::UIManager::get().create_checkbox(id);
						luaL_getmetatable(state,"UI._checkbox_metatable");
						lua_setmetatable(state,-2);
						frame->_id = id;
						memcpy(frame->_type,frame_type.c_str(),sizeof(frame->_type));
					}
					else if ( type == "image" )
					{
						unsigned int	texid = id;
						bool			uvEnabled = false;
						float			u1 = 0.0f;
						float			u2 = 0.0f;
						float			u3 = 0.0f;
						float			u4 = 0.0f;
						float			v1 = 0.0f;
						float			v2 = 0.0f;
						float			v3 = 0.0f;
						float			v4 = 0.0f;
						
						

						if ( argument_counter > 3 )
						{
							luaL_argcheck(state,lua_isnumber(state,4),4,"Texture ID argument must be an integer!");
							texid = static_cast<TEXEngine::UI::ID>(lua_tointeger(state,4));
							
							if(argument_counter > 4)
							{
								if(argument_counter == 12)
								{
									uvEnabled = true;
									luaL_argcheck(state,lua_isnumber(state,5),5,"Texture u1 argument must be a number!");
									u1 = static_cast<float>(lua_tonumber(state,5));
									luaL_argcheck(state,lua_isnumber(state,6),6,"Texture v1 argument must be a number!");
									v1 = static_cast<float>(lua_tonumber(state,6));
									luaL_argcheck(state,lua_isnumber(state,7),7,"Texture u2 argument must be a number!");
									u2 = static_cast<float>(lua_tonumber(state,7));
									luaL_argcheck(state,lua_isnumber(state,8),8,"Texture v2 argument must be a number!");
									v2 = static_cast<float>(lua_tonumber(state,8));
									luaL_argcheck(state,lua_isnumber(state,9),9,"Texture u3 argument must be a number!");
									u3 = static_cast<float>(lua_tonumber(state,9));
									luaL_argcheck(state,lua_isnumber(state,10),10,"Texture v3 argument must be a number!");
									v3 = static_cast<float>(lua_tonumber(state,10));
									luaL_argcheck(state,lua_isnumber(state,11),11,"Texture u4 argument must be a number!");
									u4 = static_cast<float>(lua_tonumber(state,11));
									luaL_argcheck(state,lua_isnumber(state,12),12,"Texture v4 argument must be a number!");
									v4 = static_cast<float>(lua_tonumber(state,12));
								}
								else
									luaL_argerror(state,5,"If you decide to pass your own UV, you have to pass all 8 of them");
							}
						}
						else
							luaL_argerror(state,4,"You have to give a texture id!");


						if ( uvEnabled ) 
							TEXEngine::Core::UIManager::get().create_image(id, texid, u1, v1, u2, v2, u3, v3, u4, v4);
						else
							TEXEngine::Core::UIManager::get().create_image(id,texid);
						
						luaL_getmetatable(state,"UI._image_metatable");
						lua_setmetatable(state,-2);
						frame->_id = id;
						memcpy(frame->_type,frame_type.c_str(),sizeof(frame->_type));
					}
					else if ( type == "label" )
					{
						TEXEngine::UI::ID	font_id = 0;



						if ( argument_counter > 3 )
						{
							luaL_argcheck(state,lua_isnumber(state,4),4,"Font ID argument must be an integer!");
							font_id = static_cast<TEXEngine::UI::ID>(lua_tointeger(state,4));
							luaL_argcheck(state,TEXEngine::Core::UIManager::get().get_font(font_id) != NULL ,4,"Invalid font id!");
						}
						else
							luaL_argerror(state,4,"You have to give a font id!");


						TEXEngine::Core::UIManager::get().create_label(id,font_id,"");
						luaL_getmetatable(state,"UI._label_metatable");
						lua_setmetatable(state,-2);
						frame->_id = id;
						memcpy(frame->_type,frame_type.c_str(),sizeof(frame->_type));
					}
					else if ( type == "slider" )
					{
						int		steps = 50;
						int		min_value = 0;
						int		max_value = 100;
					
						
						
						if ( argument_counter > 3 )
						{
							luaL_argcheck(state,lua_isnumber(state,4),4,"Steps argument for slider creation should be an integer!");
							steps = static_cast<int>(lua_tointeger(state,4));
						}

						if ( argument_counter > 4 )
						{
							luaL_argcheck(state,lua_isnumber(state,5),5,"Minimum value for slider creation should be an integer!");
							min_value = static_cast<int>(lua_tointeger(state,5));
						}

						if ( argument_counter > 5 )
						{
							luaL_argcheck(state,lua_isnumber(state,6),6,"Maximum value for slider creation should be an integer!");
							max_value = static_cast<int>(lua_tointeger(state,6));
						}

					
						TEXEngine::Core::UIManager::get().create_slider(id,steps,min_value,max_value);
						luaL_getmetatable(state,"UI._slider_metatable");
						lua_setmetatable(state,-2);
						frame->_id = id;
						memcpy(frame->_type,frame_type.c_str(),sizeof(frame->_type));
					}
					else if ( type == "textbox" )
					{
						TEXEngine::UI::ID	font_id = 0;



						if ( argument_counter > 3 )
						{
							luaL_argcheck(state,lua_isnumber(state,4),4,"Font ID argument for textbox creation must be a number!");
							font_id = static_cast<TEXEngine::UI::ID>(lua_tointeger(state,4));
							luaL_argcheck(state,TEXEngine::Core::UIManager::get().get_font(font_id) != NULL ,4,"Invalid font id!");
						}
						else
							luaL_argerror(state,4,"You have to give a font id!");


						TEXEngine::Core::UIManager::get().create_textbox(id,font_id,"");
						luaL_getmetatable(state,"UI._textbox_metatable");
						lua_setmetatable(state,-2);
						frame->_id = id;
						memcpy(frame->_type,frame_type.c_str(),sizeof(frame->_type));
					}
					else	/* container */
					{
						TEXEngine::Core::UIManager::get().create_container(id);
						luaL_getmetatable(state,"UI._container_metatable");
						lua_setmetatable(state,-2);
						frame->_id = id;
						memcpy(frame->_type,frame_type.c_str(),sizeof(frame->_type));
					}


					if ( frame_name != "" )
					{
						lua_setglobal(state,frame_name.c_str());
						lua_getglobal(state,frame_name.c_str());
					}
				}
				else
					luaL_argerror(state,1,"Invalid ID parameter!");
			}
			else
				luaL_argerror(state,1,"You must provide at least 3 arguments!");


			return 1;
		};

		int	LuaUILibrary::_create_font( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUIFrame*			frame = NULL;
				TEXEngine::UI::ID	id = 0;
				std::string			frame_name = "";



				luaL_argcheck(state,lua_isnumber(state,1),1,"ID argument must be an unsigned integer!");
					
				id = static_cast<TEXEngine::UI::ID>(lua_tointeger(state,1));

				if ( argument_counter > 1 )
				{
					luaL_argcheck(state,lua_isstring(state,2)  ||  lua_isnil(state,2) ,2,"Name argument must be a string or nil!");
				
					if ( lua_isstring(state,2) )
						frame_name += lua_tostring(state,2);
				}

			
				frame = static_cast<LuaUIFrame*>(lua_newuserdata(state,sizeof(LuaUIFrame)));

				if ( frame == NULL )
					luaL_error(state,"Font creation failed due to memory allocation problem!");


				TEXEngine::Core::UIManager::get().create_font(id);
				luaL_getmetatable(state,"UI._font_metatable");
				lua_setmetatable(state,-2);
				frame->_id = id;
				memset(frame->_type,'\0',sizeof(frame->_type));
				memcpy(frame->_type,"font",sizeof("font"));

				if ( frame_name != "" )
				{
					lua_setglobal(state,frame_name.c_str());
					lua_getglobal(state,frame_name.c_str());
				}
			}
			else
				luaL_argerror(state,1,"You must provide 1 arguments!");


			return 1;
		};


		int	LuaUILibrary::_trigger_event( lua_State* state ) 
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				TEXEngine::Core::Message	parameters(0,argument_counter-1,argument_counter-1);
				std::string*				variables = new (std::nothrow) std::string[argument_counter-1];
				unsigned int				code = 0;
				unsigned int				real_counter = 0;
				unsigned int				pointer_counter = 0;



				luaL_argcheck(state,lua_isnumber(state,1),1,"First parameter should be an unsigned integer!");
				code = static_cast<unsigned int>(lua_tointeger(state,1));

				if ( TEXEngine::Core::UIManager::get().is_event_available(code) )
				{
					for ( int i = 2;  i <= argument_counter;  ++i )
					{
						if ( lua_isnumber(state,i) )
						{
							parameters.add_real_data(real_counter,static_cast<TEXEngine::Core::MESSAGE_REAL_DATA_TYPE>(lua_tonumber(state,i)));
							++real_counter;
						}
						else if ( lua_isstring(state,i) )
						{
							if ( variables )
							{
								variables[pointer_counter] = lua_tostring(state,i);
								parameters.add_pointer(pointer_counter,static_cast<void*>(&variables[pointer_counter]));
								++pointer_counter;
							}
						}
					}

					TEXEngine::Core::trigger_event(code,parameters);
				}

				if ( variables )
					delete[] variables;
			}
			else
				luaL_argerror(state,1,"You must provide at least an event argument!");


			return 0;
		};


		int	LuaUILibrary::_bind_on_event_function( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);

				if ( argument_counter > 2 )
					lua_pop(state,argument_counter - 2 );

				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));


				if ( frame )
				{
					TEXEngine::UI::UIComponent*	found = NULL;



					luaL_argcheck(state,lua_isfunction(state,2),2,"Function argument must be a Lua function!");
							
					found = TEXEngine::Core::UIManager::get().get_component(frame->_id);

					if ( found )
					{
						std::stringstream	buffer; 
						std::string			index;

						buffer << found->get_id() << "._on_event_function";
						buffer >> index;
						lua_getglobal(state,"UI.__frame_on_event_functions");
						if ( lua_istable(state,-1) )
						{
							lua_getfield(state,-1,index.c_str());

							if ( !lua_istable(state,-1) )
							{
								lua_pop(state,1);
								lua_pushstring(state,index.c_str());
								lua_newtable(state);
								lua_settable(state,-3);
								lua_pop(state,1);
							}
							else
								lua_pop(state,2);

							lua_getglobal(state,"UI.__frame_on_event_functions");
							lua_insert(state,2);
							lua_getfield(state,-2,index.c_str());
							lua_insert(state,3);
							lua_pushstring(state,"_function");
							lua_insert(state,4);
							lua_settable(state,-3);
								
							lua_insert(state,1);
							lua_insert(state,1);
							lua_pushstring(state,"_frame");
							lua_insert(state,3);
							lua_settable(state,-3);


							found->bind(index);
						}
						else
							luaL_argerror(state,1,"Global table was not initialised!");
					}
					else
						luaL_argerror(state,1,"The frame given was not found!");
				}
				else
					luaL_argerror(state,1,"The frame given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a frame argument!");


			return 0;
		};

		int	LuaUILibrary::_set_frame_size( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					float						width = 0;
					float						height = 0;
					bool						width_active = false;
					bool						height_active = false;
					TEXEngine::UI::UIComponent*	found = NULL;



					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isnumber(state,2),2,"Width argument must be a number greater than 0!");
						width = static_cast<float>(lua_tonumber(state,2));
						width_active = true;
					}
	
					if ( argument_counter > 2 )
					{
						luaL_argcheck(state,lua_isnumber(state,3),3,"Height argument must be a number greater that 0!");
						height = static_cast<float>(lua_tonumber(state,3));
						height_active = true;
					}


					found = TEXEngine::Core::UIManager::get().get_component(frame->_id);

					if ( found )
					{
						if ( width_active )
							found->set_width(width);
	
						if ( height_active )
							found->set_height(height);
					}
					else
						luaL_argerror(state,1,"The frame given was not found!");
				}
				else
					luaL_argerror(state,1,"The frame given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a frame argument!");


			return 0;
		};

		int	LuaUILibrary::_set_frame_position( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					float						x = 0;
					float						y = 0;
					bool						x_active = false;
					bool						y_active = false;
					TEXEngine::UI::UIComponent* found = NULL;



					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isnumber(state,2),2,"X position argument must be a number inside [0,1]!");
						x = static_cast<float>(lua_tonumber(state,2));
						x_active = true;
					}

					if ( argument_counter > 2 )
					{
						luaL_argcheck(state,lua_isnumber(state,3),3,"Y position argument must be a number inside [0,1]!");
						y = static_cast<float>(lua_tonumber(state,3));
						y_active = true;
					}

					found = TEXEngine::Core::UIManager::get().get_component(frame->_id);

					if ( found )
					{
						if ( x_active )
							found->set_x(x);
	
						if ( y_active )
							found->set_y(y);
					}
					else
						luaL_argerror(state,1,"The frame given was not found!");
				}
				else
					luaL_argerror(state,1,"The frame given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a frame argument!");


			return 0;
		};

		int	LuaUILibrary::_set_background_colour( lua_State* state )	
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					float						red = 0;
					float						green = 0;
					float						blue = 0;
					float						alpha = 0;
					bool						red_active = false;
					bool						green_active = false;
					bool						blue_active = false;
					bool						alpha_active = false;
					TEXEngine::UI::UIComponent*	found = NULL;



					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isnumber(state,2),2,"Background colour argument must be an number!");
						red = static_cast<float>(lua_tonumber(state,2));
						red_active = true;
					}

					if ( argument_counter > 2 )
					{
						luaL_argcheck(state,lua_isnumber(state,3),3,"Background colour argument must be an number!");
						green = static_cast<float>(lua_tonumber(state,3));
						green_active = true;
					}

					if ( argument_counter > 3 )
					{
						luaL_argcheck(state,lua_isnumber(state,4),4,"Background colour argument must be an number!");
						blue = static_cast<float>(lua_tonumber(state,4));
						blue_active = true;
					}

					if ( argument_counter > 4 )
					{
						luaL_argcheck(state,lua_isnumber(state,5),5,"Background colour argument must be an number!");
						alpha = static_cast<float>(lua_tonumber(state,5));
						alpha_active = true;
					}

					found = TEXEngine::Core::UIManager::get().get_component(frame->_id);

					if ( found )
					{
						TEXEngine::Util::ColorRGBAf	colour(found->get_background_color());



						if ( red_active )
							colour.red = red;

						if ( green_active )
							colour.green = green;

						if ( blue_active )
							colour.blue = blue;

						if ( alpha_active )
							colour.alpha = alpha;

						found->set_background(colour);
					}
					else
						luaL_argerror(state,1,"The frame given was not found!");
				}
				else
					luaL_argerror(state,1,"The frame given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a frame argument!");


			return 0;
		};

		int	LuaUILibrary::_set_border_thickness( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					float						thickness = 0;
					bool						thickness_active = false;
					TEXEngine::UI::UIComponent*	found = NULL;



					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isnumber(state,2),2,"Border thickness argument must be a number!");
						thickness = static_cast<float>(lua_tonumber(state,2));
						thickness_active = true;
					}

					found = TEXEngine::Core::UIManager::get().get_component(frame->_id);

					if ( found )
					{
						if ( thickness_active )
							found->set_border_thickness(thickness);
					}
					else
						luaL_argerror(state,1,"The frame given was not found!");
				}
				else
					luaL_argerror(state,1,"The frame given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a frame argument!");


			return 0;
		};

		int	LuaUILibrary::_set_border_colour( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					float						red = 1;
					float						green = 1;
					float						blue = 1;
					float						alpha = 1;
					bool						active = false;
					TEXEngine::UI::UIComponent*	found = NULL;



					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isnumber(state,2),2,"Background colour argument must be an number!");
						red = static_cast<float>(lua_tonumber(state,2));
						active = true;
					}

					if ( argument_counter > 2 )
					{
						luaL_argcheck(state,lua_isnumber(state,3),3,"Background colour argument must be an number!");
						green = static_cast<float>(lua_tonumber(state,3));
					}

					if ( argument_counter > 3 )
					{
						luaL_argcheck(state,lua_isnumber(state,4),4,"Background colour argument must be an number!");
						blue = static_cast<float>(lua_tonumber(state,4));
					}

					if ( argument_counter > 4 )
					{
						luaL_argcheck(state,lua_isnumber(state,5),5,"Background colour argument must be an number!");
						alpha = static_cast<float>(lua_tonumber(state,5));
					}

					found = TEXEngine::Core::UIManager::get().get_component(frame->_id);

					if ( found )
					{
						if ( active )
							found->set_border_color(TEXEngine::Util::ColorRGBAf(red,green,blue,alpha));
					}
					else
						luaL_argerror(state,1,"The frame given was not found!");
				}
				else
					luaL_argerror(state,1,"The frame given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a frame argument!");


			return 0;
		};

		int	LuaUILibrary::_set_visible( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					bool						visible = false;
					bool						visible_active = false;
					TEXEngine::UI::UIComponent*	found = NULL;



					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isboolean(state,2),2,"Visible argument must be a boolean!");
						visible = ( lua_toboolean(state,2)  ?  true : false );
						visible_active = true;
					}

					found = TEXEngine::Core::UIManager::get().get_component(frame->_id);

					if ( found )
					{
						if ( visible_active )
							found->set_visible(visible);
					}
					else
						luaL_argerror(state,1,"The frame given was not found!");
				}
				else
					luaL_argerror(state,1,"The frame given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a frame argument!");


			return 0;
		};

		int	LuaUILibrary::_set_enabled( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					bool						enabled = false;
					bool						enabled_active = false;
					TEXEngine::UI::UIComponent*	found = NULL;



					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isboolean(state,2),2,"Enable argument must be a boolean!");
						enabled = ( lua_toboolean(state,2)  ?  true : false );
						enabled_active = true;
					}

					found = TEXEngine::Core::UIManager::get().get_component(frame->_id);

					if ( found )
					{
						if ( enabled_active )
							found->set_enabled(enabled);
					}
					else
						luaL_argerror(state,1,"The frame given was not found!");
				}
				else
					luaL_argerror(state,1,"The frame given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a frame argument!");


			return 0;
		};

		int	LuaUILibrary::_set_render_border( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					bool						render = false;
					bool						render_active = false;
					TEXEngine::UI::UIComponent*	found = NULL;



					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isboolean(state,2),2,"Render border argument must be a boolean!");
						render = ( lua_toboolean(state,2)  ?  true : false );
						render_active = true;
					}

					found = TEXEngine::Core::UIManager::get().get_component(frame->_id);

					if ( found )
					{
						if ( render_active )
							found->set_render_border(render);
					}
					else
						luaL_argerror(state,1,"The frame given was not found!");
				}
				else
					luaL_argerror(state,1,"The frame given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a frame argument!");


			return 0;
		};

		int	LuaUILibrary::_set_tab_index( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					unsigned int				index = 0;
					bool						index_active = false;
					TEXEngine::UI::UIComponent*	found = NULL;



					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isnumber(state,2),2,"Tab index argument must be an unsigned number!");
						index = static_cast<unsigned int>(lua_tointeger(state,2));
						index_active = true;
					}

					found = TEXEngine::Core::UIManager::get().get_component(frame->_id);

					if ( found )
					{
						if ( index_active )
							found->set_tabindex(index);
					}
					else
						luaL_argerror(state,1,"The frame given was not found!");
				}
				else
					luaL_argerror(state,1,"The frame given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a frame argument!");


			return 0;
		};
		
		int	LuaUILibrary::_get_id( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::UIComponent*	found = TEXEngine::Core::UIManager::get().get_component(frame->_id);;



					if ( found )
						lua_pushinteger(state,static_cast<lua_Integer>(found->get_id()));
					else
						luaL_argerror(state,1,"The frame given was not found!");
				}
				else
					luaL_argerror(state,1,"The frame given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a frame argument!");


			return 1;
		};

		int	LuaUILibrary::_get_frame_size( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::UIComponent*	found = TEXEngine::Core::UIManager::get().get_component(frame->_id);;



					if ( found )
					{
						lua_pushnumber(state,static_cast<lua_Number>(found->get_width()));
						lua_pushnumber(state,static_cast<lua_Number>(found->get_height()));
					}
					else
						luaL_argerror(state,1,"The frame given was not found!");
				}
				else
					luaL_argerror(state,1,"The frame given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a frame argument!");


			return 2;
		};

		int	LuaUILibrary::_get_frame_position( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::UIComponent*	found = TEXEngine::Core::UIManager::get().get_component(frame->_id);



					if ( found )
					{
						lua_pushnumber(state,static_cast<lua_Number>(found->get_x()));
						lua_pushnumber(state,static_cast<lua_Number>(found->get_y()));
					}
					else
						luaL_argerror(state,1,"The frame given was not found!");
				}
				else
					luaL_argerror(state,1,"The frame given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a frame argument!");


			return 2;
		};

		int	LuaUILibrary::_get_background_colour( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::UIComponent*	found = TEXEngine::Core::UIManager::get().get_component(frame->_id);



					if ( found )
					{
						TEXEngine::Util::ColorRGBAf	colour(found->get_background_color());



						lua_pushnumber(state,static_cast<lua_Number>(colour.red));
						lua_pushnumber(state,static_cast<lua_Number>(colour.green));
						lua_pushnumber(state,static_cast<lua_Number>(colour.blue));
						lua_pushnumber(state,static_cast<lua_Number>(colour.alpha));
					}
					else
						luaL_argerror(state,1,"The frame given was not found!");
				}
				else
					luaL_argerror(state,1,"The frame given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a frame argument!");


			return 4;
		};

		int	LuaUILibrary::_is_visible( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::UIComponent*	found = TEXEngine::Core::UIManager::get().get_component(frame->_id);;



					if ( found )
						lua_pushboolean(state,static_cast<int>(( found->is_visible()  ?  1 : 0 )));
					else
						luaL_argerror(state,1,"The frame given was not found!");
				}
				else
					luaL_argerror(state,1,"The frame given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a frame argument!");


			return 1;
		};

	int	LuaUILibrary::_is_enabled( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::UIComponent*	found = TEXEngine::Core::UIManager::get().get_component(frame->_id);;



					if ( found )
						lua_pushboolean(state,static_cast<int>(( found->is_enabled()  ?  1 : 0 )));
					else
						luaL_argerror(state,1,"The frame given was not found!");
				}
				else
					luaL_argerror(state,1,"The frame given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a frame argument!");


			return 1;
		};


		int	LuaUILibrary::_button_is_down( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::Button*	found = TEXEngine::Core::UIManager::get().get_button(frame->_id);;



					if ( found )
						lua_pushboolean(state,static_cast<int>(( found->is_down()  ?  1 : 0 )));
					else
						luaL_argerror(state,1,"The button given was not found!");
				}
				else
					luaL_argerror(state,1,"The button given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a button argument!");


			return 1;
		};


		int	LuaUILibrary::_checkbox_set_checked( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::Checkbox*	found = NULL;
					bool						value = false;
					bool						value_active = false;



					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isboolean(state,2),2,"Checked value argument must be a boolean!");
						value = ( lua_toboolean(state,2)  ?  true : false );
						value_active = true;
					}

					found = TEXEngine::Core::UIManager::get().get_checkbox(frame->_id);

					if ( found )
					{
						if ( value_active )
							found->set_checked(value);
					}
					else
						luaL_argerror(state,1,"The checkbox given was not found!");
				}
				else
					luaL_argerror(state,1,"The checkbox given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a checkbox argument!");


			return 0;
		};

		int	LuaUILibrary::_checkbox_set_pixel_size( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::Checkbox*	found = NULL;
					float						size = 0;
					bool						size_active = false;



					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isnumber(state,2),2,"Pixel size argument must be a number!");
						size = static_cast<float>(lua_tonumber(state,2));
						size_active = true;
					}

					found = TEXEngine::Core::UIManager::get().get_checkbox(frame->_id);

					if ( found )
					{
						if ( size_active )
							found->set_pixel_size(size);
					}
					else
						luaL_argerror(state,1,"The checkbox given was not found!");
				}
				else
					luaL_argerror(state,1,"The checkbox given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a checkbox argument!");


			return 0;
		};

		int	LuaUILibrary::_checkbox_set_cross_colour( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::Checkbox*	found = NULL;
					float						red = 0;
					float						green = 0;
					float						blue = 0;
					float						alpha = 0;
					bool						red_active = false;
					bool						green_active = false;
					bool						blue_active = false;
					bool						alpha_active = false;



					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isnumber(state,2),2,"Background colour argument must be an number!");
						red = static_cast<float>(lua_tonumber(state,2));
						red_active = true;
					}

					if ( argument_counter > 2 )
					{
						luaL_argcheck(state,lua_isnumber(state,3),3,"Background colour argument must be an number!");
						green = static_cast<float>(lua_tonumber(state,3));
						green_active = true;
					}

					if ( argument_counter > 3 )
					{
						luaL_argcheck(state,lua_isnumber(state,4),4,"Background colour argument must be an number!");
						blue = static_cast<float>(lua_tonumber(state,4));
						blue_active = true;
					}

					if ( argument_counter > 4 )
					{
						luaL_argcheck(state,lua_isnumber(state,5),5,"Background colour argument must be an number!");
						alpha = static_cast<float>(lua_tonumber(state,5));
						alpha_active = true;
					}

					found = TEXEngine::Core::UIManager::get().get_checkbox(frame->_id);

					if ( found )
					{
						TEXEngine::Util::ColorRGBAf	colour(found->get_background_color());



						if ( red_active )
							colour.red = red;

						if ( green_active )
							colour.green = green;

						if ( blue_active )
							colour.blue = blue;

						if ( alpha_active )
							colour.alpha = alpha;

						found->set_background(colour);
					}
					else
						luaL_argerror(state,1,"The checkbox given was not found!");
				}
				else
					luaL_argerror(state,1,"The checkbox given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a checkbox argument!");


			return 0;
		};

		int	LuaUILibrary::_checkbox_is_checked( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::Checkbox*	found = TEXEngine::Core::UIManager::get().get_checkbox(frame->_id);



					if ( found )
						lua_pushboolean(state,static_cast<int>(( found->is_checked()  ?  1 : 0 )));
					else
						luaL_argerror(state,1,"The checkbox given was not found!");
				}
				else
					luaL_argerror(state,1,"The checkbox given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a checkbox argument!");


			return 1;
		};

		int	LuaUILibrary::_checkbox_get_pixel_size( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::Checkbox*	found = TEXEngine::Core::UIManager::get().get_checkbox(frame->_id);



					if ( found )
						lua_pushnumber(state,static_cast<lua_Number>(found->get_pixel_size()));
					else
						luaL_argerror(state,1,"The checkbox given was not found!");
				}
				else
					luaL_argerror(state,1,"The checkbox given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a checkbox argument!");


			return 1;
		};

		int	LuaUILibrary::_checkbox_get_cross_colour( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::Checkbox*	found = TEXEngine::Core::UIManager::get().get_checkbox(frame->_id);;



					if ( found )
					{
						TEXEngine::Util::ColorRGBAf	colour(found->get_cross_color());



						lua_pushnumber(state,static_cast<lua_Number>(colour.red));
						lua_pushnumber(state,static_cast<lua_Number>(colour.green));
						lua_pushnumber(state,static_cast<lua_Number>(colour.blue));
						lua_pushnumber(state,static_cast<lua_Number>(colour.alpha));
					}
					else
						luaL_argerror(state,1,"The checkbox given was not found!");
				}
				else
					luaL_argerror(state,1,"The checkbox given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a checkbox argument!");


			return 4;
		};


		int	LuaUILibrary::_container_attach( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::Container*	found = NULL;
					TEXEngine::UI::UIComponent*	child_found = NULL;
					LuaUIFrame*					child_frame = NULL;
					ZIndex						z_value = 0;
					bool						z_active = false;



					if ( argument_counter > 1 )
					{
						LuaUILibrary::_check_frame_function_args(state,2);
						child_frame = static_cast<LuaUIFrame*>(lua_touserdata(state,2));
					}

					if ( argument_counter > 2 )
					{
						luaL_argcheck(state,lua_isnumber(state,3),3,"The second parameter should be an integer!");
						z_value = static_cast<ZIndex>(lua_tointeger(state,3));
						z_active = true;
					}

					if ( child_frame )
					{
						found = TEXEngine::Core::UIManager::get().get_container(frame->_id);
						child_found = TEXEngine::Core::UIManager::get().get_component(child_frame->_id);

						if ( found )
						{
							if ( child_found )
							{
								if ( z_active )
									found->attach(child_found->get_id(),z_value);
								else
									found->attach(child_found->get_id());
							}
							else
								luaL_argerror(state,2,"The child frame given was not found!");
						}
						else
							luaL_argerror(state,1,"The container given was not found!");
					}
					else
						luaL_argerror(state,1,"The child frame given could not be verified!");
				}
				else
					luaL_argerror(state,1,"The container given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a container argument!");


			return 0;
		};

		int	LuaUILibrary::_container_detach( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::Container*	found = NULL;
					TEXEngine::UI::UIComponent*	child_found = NULL;
					LuaUIFrame*					child_frame = NULL;



					if ( argument_counter > 1 )
					{
						LuaUILibrary::_check_frame_function_args(state,2);
						child_frame = static_cast<LuaUIFrame*>(lua_touserdata(state,2));
					}


					if ( child_frame )
					{
						found = TEXEngine::Core::UIManager::get().get_container(frame->_id);
						child_found = TEXEngine::Core::UIManager::get().get_component(child_frame->_id);

						if ( found )
						{
							if ( child_found )
								found->detach(child_found->get_id());
							else
								luaL_argerror(state,2,"The child frame given was not found!");
						}
						else
							luaL_argerror(state,1,"The container given was not found!");
					}
					else
						luaL_argerror(state,1,"The child frame given could not be verified!");
				}
				else
					luaL_argerror(state,1,"The container given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a container argument!");


			return 0;
		};

		int	LuaUILibrary::_container_set_renderable( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::Container*	found = NULL;
					bool						renderable = false;
					bool						active = false;



					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isboolean(state,2),2,"Render argument must be a boolean!");
						renderable = ( lua_toboolean(state,2)  ?  true : false );
						active = true;
					}


					found = TEXEngine::Core::UIManager::get().get_container(frame->_id);

					if ( found )
					{
						if ( active )
							found->set_renderable(renderable);
					}
					else
						luaL_argerror(state,1,"The container given was not found!");
				}
				else
					luaL_argerror(state,1,"The container given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a container argument!");


			return 0;
		};

		int	LuaUILibrary::_container_set_z_position( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::Container*	found = NULL;
					int							z = 0;
					bool						active = false;



					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isnumber(state,2),2,"Z argument must be a number!");
						z = static_cast<int>(lua_tointeger(state,2));
						active = true;
					}


					found = TEXEngine::Core::UIManager::get().get_container(frame->_id);

					if ( found )
					{
						if ( active )
							TEXEngine::Core::UIManager::get().set_container_z(found->get_id(),z);
					}
					else
						luaL_argerror(state,1,"The container given was not found!");
				}
				else
					luaL_argerror(state,1,"The container given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a container argument!");


			return 0;
		}


		int	LuaUILibrary::_image_set_texu_shift( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::UIImage*	found = NULL;
					float					begin_shift = 0;
					float					end_shift = 0;
					bool					begin_active = false;
					bool					end_active = false;



					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isnumber(state,2),2,"Begin shift argument must be a number!");
						begin_shift = static_cast<float>(lua_tonumber(state,2));
						begin_active = true;
					}

					if ( argument_counter > 2 )
					{
						luaL_argcheck(state,lua_isnumber(state,3),3,"End shift argument must be a number!");
						end_shift = static_cast<float>(lua_tonumber(state,3));
						end_active = true;
					}


					found = TEXEngine::Core::UIManager::get().get_image(frame->_id);

					if ( found )
					{
						float	begin = found->get_texu_shift_start();
						float	end = found->get_texu_shift_end();




						if ( begin_active )
							begin = begin_shift;

						if ( end_active )
							end = end_shift;

						found->set_texu_shift(begin,end);
					}
					else
						luaL_argerror(state,1,"The image given was not found!");
				}
				else
					luaL_argerror(state,1,"The image given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide an image argument!");


			return 0;
		};

		int	LuaUILibrary::_image_set_texv_shift( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::UIImage*	found = NULL;
					float					begin_shift = 0;
					float					end_shift = 0;
					bool					begin_active = false;
					bool					end_active = false;



					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isnumber(state,2),2,"Begin shift argument must be a number!");
						begin_shift = static_cast<float>(lua_tonumber(state,2));
						begin_active = true;
					}

					if ( argument_counter > 2 )
					{
						luaL_argcheck(state,lua_isnumber(state,3),3,"End shift argument must be a number!");
						end_shift = static_cast<float>(lua_tonumber(state,3));
						end_active = true;
					}


					found = TEXEngine::Core::UIManager::get().get_image(frame->_id);

					if ( found )
					{
						float	begin = found->get_texv_shift_start();
						float	end = found->get_texv_shift_end();




						if ( begin_active )
							begin = begin_shift;

						if ( end_active )
							end = end_shift;

						found->set_texv_shift(begin,end);
					}
					else
						luaL_argerror(state,1,"The image given was not found!");
				}
				else
					luaL_argerror(state,1,"The image given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide an image argument!");


			return 0;
		};

		int	LuaUILibrary::_image_get_texu_shift( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::UIImage*	found = TEXEngine::Core::UIManager::get().get_image(frame->_id);



					if ( found )
					{
						lua_pushnumber(state,static_cast<lua_Number>(found->get_texu_shift_start()));
						lua_pushnumber(state,static_cast<lua_Number>(found->get_texu_shift_end()));
					}
					else
						luaL_argerror(state,1,"The image given was not found!");
				}
				else
					luaL_argerror(state,1,"The image given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide an image argument!");


			return 2;
		};

		int	LuaUILibrary::_image_get_texv_shift( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::UIImage*	found = TEXEngine::Core::UIManager::get().get_image(frame->_id);;



					if ( found )
					{
						lua_pushnumber(state,static_cast<lua_Number>(found->get_texv_shift_start()));
						lua_pushnumber(state,static_cast<lua_Number>(found->get_texv_shift_end()));
					}
					else
						luaL_argerror(state,1,"The image given was not found!");
				}
				else
					luaL_argerror(state,1,"The image given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide an image argument!");


			return 2;
		};


		int	LuaUILibrary::_slider_set_value( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::Slider*	found = NULL;
					int						value = 0;
					bool					active = false;



					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isnumber(state,2),2,"Value argument must be an integer!");
						value = static_cast<int>(lua_tointeger(state,2));
						active = true;
					}


					found = TEXEngine::Core::UIManager::get().get_slider(frame->_id);

					if ( found )
					{
						if ( active )
							found->set_value(value);
					}
					else
						luaL_argerror(state,1,"The slider given was not found!");
				}
				else
					luaL_argerror(state,1,"The slider given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a slider argument!");


			return 0;
		};

		int	LuaUILibrary::_slider_set_marker_colour( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					float						red = 1;
					float						green = 1;
					float						blue = 1;
					float						alpha = 1;
					bool						active = false;
					TEXEngine::UI::Slider*		found = NULL;



					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isnumber(state,2),2,"Background colour argument must be an number!");
						red = static_cast<float>(lua_tonumber(state,2));
						active = true;
					}

					if ( argument_counter > 2 )
					{
						luaL_argcheck(state,lua_isnumber(state,3),3,"Background colour argument must be an number!");
						green = static_cast<float>(lua_tonumber(state,3));
					}

					if ( argument_counter > 3 )
					{
						luaL_argcheck(state,lua_isnumber(state,4),4,"Background colour argument must be an number!");
						blue = static_cast<float>(lua_tonumber(state,4));
					}

					if ( argument_counter > 4 )
					{
						luaL_argcheck(state,lua_isnumber(state,5),5,"Background colour argument must be an number!");
						alpha = static_cast<float>(lua_tonumber(state,5));
					}



					found = TEXEngine::Core::UIManager::get().get_slider(frame->_id);

					if ( found )
					{
						if ( active )
							found->set_marker_color(TEXEngine::Util::ColorRGBAf(red,green,blue,alpha));
					}
					else
						luaL_argerror(state,1,"The slider given was not found!");
				}
				else
					luaL_argerror(state,1,"The slider given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a slider argument!");


			return 0;
		};

		int	LuaUILibrary::_slider_set_marker_width( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::Slider*	found = NULL;
					float					value = 0;
					bool					active = false;



					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isnumber(state,2),2,"Width argument must be a number!");
						value = static_cast<float>(lua_tonumber(state,2));
						active = true;
					}


					found = TEXEngine::Core::UIManager::get().get_slider(frame->_id);

					if ( found )
					{
						if ( active )
							found->set_marker_width(value);
					}
					else
						luaL_argerror(state,1,"The slider given was not found!");
				}
				else
					luaL_argerror(state,1,"The slider given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a slider argument!");


			return 0;
		};

		int	LuaUILibrary::_slider_set_marker_padding( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::Slider*	found = NULL;
					float					width_padding = 0;
					float					height_padding = 0;
					bool					width_active = false;
					bool					height_active = false;



					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isnumber(state,2),2,"Width padding argument must be a float!");
						width_padding = static_cast<float>(lua_tonumber(state,2));
						width_active = true;
					}

					if ( argument_counter > 2 )
					{
						luaL_argcheck(state,lua_isnumber(state,3),3,"Height argument must be a float!");
						height_padding = static_cast<float>(lua_tonumber(state,3));
						height_active = true;
					}


					found = TEXEngine::Core::UIManager::get().get_slider(frame->_id);

					if ( found )
					{
						if ( width_active )
							found->set_marker_width_padding(width_padding);

						if ( height_active )
							found->set_marker_height_padding(height_padding);
					}
					else
						luaL_argerror(state,1,"The slider given was not found!");
				}
				else
					luaL_argerror(state,1,"The slider given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a slider argument!");


			return 0;
		};

		int	LuaUILibrary::_slider_get_value( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::Slider*	found = TEXEngine::Core::UIManager::get().get_slider(frame->_id);


					
					if ( found )
						lua_pushinteger(state,static_cast<lua_Integer>(found->get_value()));
					else
						luaL_argerror(state,1,"The slider given was not found!");
				}
				else
					luaL_argerror(state,1,"The slider given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a slider argument!");


			return 1;
		};


		int	LuaUILibrary::_textbox_set_blink_delay( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::Textbox*	found = NULL;
					double					value = 0;
					bool					active = false;



					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isnumber(state,2),2,"Blink delay argument must be a number!");
						value = static_cast<double>(lua_tonumber(state,2));
						active = true;
					}


					found = TEXEngine::Core::UIManager::get().get_textbox(frame->_id);

					if ( found )
					{
						if ( active )
							found->set_blink_delay(value);
					}
					else
						luaL_argerror(state,1,"The textbox given was not found!");
				}
				else
					luaL_argerror(state,1,"The textbox given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a textbox argument!");


			return 0;
		};

		int	LuaUILibrary::_textbox_set_editable( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::Textbox*	found = NULL;
					bool					value = false;
					bool					active = false;



					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isboolean(state,2),2,"Editable argument must be a boolean!");
						value = (lua_toboolean(state,2)  ?  true : false );
						active = true;
					}


					found = TEXEngine::Core::UIManager::get().get_textbox(frame->_id);

					if ( found )
					{
						if ( active )
							found->set_editable(value);
					}
					else
						luaL_argerror(state,1,"The textbox given was not found!");
				}
				else
					luaL_argerror(state,1,"The textbox given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a textbox argument!");


			return 0;
		};

		int	LuaUILibrary::_textbox_set_multiline( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::Textbox*	found = NULL;
					bool					value = false;
					bool					active = false;



					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isboolean(state,2),2,"Multiline argument must be a boolean!");
						value = (lua_toboolean(state,2)  ?  true : false );
						active = true;
					}


					found = TEXEngine::Core::UIManager::get().get_textbox(frame->_id);

					if ( found )
					{
						if ( active )
							found->set_multiline(value);
					}
					else
						luaL_argerror(state,1,"The textbox given was not found!");
				}
				else
					luaL_argerror(state,1,"The textbox given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a textbox argument!");


			return 0;
		};

		int	LuaUILibrary::_textbox_set_wrapping( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::Textbox*	found = NULL;
					bool					value = false;
					bool					active = false;



					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isboolean(state,2),2,"Wrapping argument must be a boolean!");
						value = (lua_toboolean(state,2)  ?  true : false );
						active = true;
					}


					found = TEXEngine::Core::UIManager::get().get_textbox(frame->_id);

					if ( found )
					{
						if ( active )
							found->set_wrapping(value);
					}
					else
						luaL_argerror(state,1,"The textbox given was not found!");
				}
				else
					luaL_argerror(state,1,"The textbox given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a textbox argument!");


			return 0;
		};

		int	LuaUILibrary::_textbox_is_editable( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::Textbox*	found = TEXEngine::Core::UIManager::get().get_textbox(frame->_id);


										
					if ( found )
						lua_pushboolean(state,static_cast<int>(( found->is_editable()  ?  1 : 0 )));
					else
						luaL_argerror(state,1,"The textbox given was not found!");
				}
				else
					luaL_argerror(state,1,"The textbox given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a textbox argument!");


			return 1;
		};


		int	LuaUILibrary::_textbox_set_caret_colour( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::Textbox*		found = NULL;
					float						red = 0;
					float						green = 0;
					float						blue = 0;
					float						alpha = 0;
					bool						red_active = false;
					bool						green_active = false;
					bool						blue_active = false;
					bool						alpha_active = false;



					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isnumber(state,2),2,"Background colour argument must be an number!");
						red = static_cast<float>(lua_tonumber(state,2));
						red_active = true;
					}

					if ( argument_counter > 2 )
					{
						luaL_argcheck(state,lua_isnumber(state,3),3,"Background colour argument must be an number!");
						green = static_cast<float>(lua_tonumber(state,3));
						green_active = true;
					}

					if ( argument_counter > 3 )
					{
						luaL_argcheck(state,lua_isnumber(state,4),4,"Background colour argument must be an number!");
						blue = static_cast<float>(lua_tonumber(state,4));
						blue_active = true;
					}

					if ( argument_counter > 4 )
					{
						luaL_argcheck(state,lua_isnumber(state,5),5,"Background colour argument must be an number!");
						alpha = static_cast<float>(lua_tonumber(state,5));
						alpha_active = true;
					}

					found = TEXEngine::Core::UIManager::get().get_textbox(frame->_id);

					if ( found )
					{
						TEXEngine::Util::ColorRGBAf	colour(found->get_caret_color());



						if ( red_active )
							colour.red = red;

						if ( green_active )
							colour.green = green;

						if ( blue_active )
							colour.blue = blue;

						if ( alpha_active )
							colour.alpha = alpha;

						found->set_caret_color(colour);
					}
					else
						luaL_argerror(state,1,"The textbox given was not found!");
				}
				else
					luaL_argerror(state,1,"The textbox given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a textbox argument!");


			return 0;
		};

		int	LuaUILibrary::_textbox_get_caret_colour( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::Textbox*		found = TEXEngine::Core::UIManager::get().get_textbox(frame->_id);;



					if ( found )
					{
						TEXEngine::Util::ColorRGBAf	colour(found->get_caret_color());



						lua_pushnumber(state,static_cast<lua_Number>(colour.red));
						lua_pushnumber(state,static_cast<lua_Number>(colour.green));
						lua_pushnumber(state,static_cast<lua_Number>(colour.blue));
						lua_pushnumber(state,static_cast<lua_Number>(colour.alpha));
					}
					else
						luaL_argerror(state,1,"The textbox given was not found!");
				}
				else
					luaL_argerror(state,1,"The textbox given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a textbox argument!");


			return 4;
		};

		int	LuaUILibrary::_textbox_set_letter_height( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::Textbox*		found = NULL;
					float						height = 0.0f;
					bool						height_active = false;



					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isnumber(state,2),2,"Letter height argument must be a number!");
						height = static_cast<float>(lua_tonumber(state,2));
						height_active = true;
					}

					found = TEXEngine::Core::UIManager::get().get_textbox(frame->_id);

					if ( found )
					{
						if ( height_active )
							found->set_letter_height(height);
					}
					else
						luaL_argerror(state,1,"The textbox given was not found!");
				}
				else
					luaL_argerror(state,1,"The textbox given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a textbox argument!");


			return 0;
		};

		int	LuaUILibrary::_textbox_get_letter_height( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::Textbox*		found = TEXEngine::Core::UIManager::get().get_textbox(frame->_id);



					if ( found )
						lua_pushnumber(state,static_cast<lua_Number>(found->get_letter_height()));
					else
						luaL_argerror(state,1,"The textbox given was not found!");
				}
				else
					luaL_argerror(state,1,"The textbox given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a textbox argument!");


			return 1;
		};

		int	LuaUILibrary::_text_component_set_text( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					std::string						text = "";
					bool							text_active = false;
					TEXEngine::UI::TextComponent*	found = NULL;
					std::string						type(frame->_type);



					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isstring(state,2),2,"Text argument must be a string!");
						text = lua_tostring(state,2);
						text_active = true;
					}



					if ( type == "label" )
						found = static_cast<TextComponent*>(TEXEngine::Core::UIManager::get().get_label(frame->_id));
					else
						found = static_cast<TextComponent*>(TEXEngine::Core::UIManager::get().get_textbox(frame->_id));

					if ( found )
					{
						if ( text_active )
							found->set_text(text.c_str());
					}
					else
						luaL_argerror(state,1,"The text component given was not found!");
				}
				else
					luaL_argerror(state,1,"The text component given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a text component argument!");


			return 0;
		};

		int	LuaUILibrary::_text_component_set_font_id( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 1 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::TextComponent*	found = NULL;
					TEXEngine::UI::ID				id = 0;



					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isnumber(state,2),2,"Font ID argument must be an integer!");
						id = static_cast<TEXEngine::UI::ID>(lua_tointeger(state,2));
					}


					if ( TEXEngine::Core::UIManager::get().get_font(id) != NULL )
					{
						std::string	type(frame->_type);



						if ( type == "label" )
							found = static_cast<TextComponent*>(TEXEngine::Core::UIManager::get().get_label(frame->_id));
						else
							found = static_cast<TextComponent*>(TEXEngine::Core::UIManager::get().get_textbox(frame->_id));

						if ( found )
								found->set_font_id(id);
						else
							luaL_argerror(state,1,"The text component given was not found!");
					}
					else
						luaL_argerror(state,1,"The font id given could not be verified!");
				}
				else
					luaL_argerror(state,1,"The text component given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a font id!");


			return 0;
		};

		int	LuaUILibrary::_text_component_get_text( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::TextComponent*	found = NULL;
					std::string						type(frame->_type);



					if ( type == "label" )
						found = static_cast<TextComponent*>(TEXEngine::Core::UIManager::get().get_label(frame->_id));
					else
						found = static_cast<TextComponent*>(TEXEngine::Core::UIManager::get().get_textbox(frame->_id));

					if ( found )
						lua_pushstring(state,found->get_text());
					else
						luaL_argerror(state,1,"The text component given was not found!");
				}
				else
					luaL_argerror(state,1,"The text component given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a text component argument!");


			return 1;
		};

			int	LuaUILibrary::_text_component_get_font_id( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::TextComponent*	found = NULL;
					std::string						type(frame->_type);




					if ( type == "label" )
						found = static_cast<TextComponent*>(TEXEngine::Core::UIManager::get().get_label(frame->_id));
					else
						found = static_cast<TextComponent*>(TEXEngine::Core::UIManager::get().get_textbox(frame->_id));

					if ( found )
						lua_pushinteger(state,static_cast<lua_Integer>(found->get_font_id()));
					else
						luaL_argerror(state,1,"The text component given was not found!");
				}
				else
					luaL_argerror(state,1,"The text component given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a text component argument!");


			return 1;
		};

		int	LuaUILibrary::_text_component_set_horiz_align( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::TextComponent*						found = NULL;
					TEXEngine::UI::TextComponent::TEXENG_TEXT_ALIGN		value = TextComponent::TEXENG_ALIGN_CENTER;
					bool												active = false;
					std::string											type(frame->_type);


					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isnumber(state,2),2,"Horizontal Align argument must be an integer!");
						value = static_cast<TEXEngine::UI::TextComponent::TEXENG_TEXT_ALIGN>(lua_tointeger(state,2));
						active = true;
					}


					if ( type == "label" )
						found = static_cast<TextComponent*>(TEXEngine::Core::UIManager::get().get_label(frame->_id));
					else
						found = static_cast<TextComponent*>(TEXEngine::Core::UIManager::get().get_textbox(frame->_id));

					if ( found )
					{
						if ( active )
							found->set_horizontal_align(value);
					}
					else
						luaL_argerror(state,1,"The text component given was not found!");
				}
				else
					luaL_argerror(state,1,"The text component given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a text component argument!");


			return 0;
		};

			int	LuaUILibrary::_text_component_get_horiz_align( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::TextComponent*	found = NULL;
					std::string						type(frame->_type);




					if ( type == "label" )
						found = static_cast<TextComponent*>(TEXEngine::Core::UIManager::get().get_label(frame->_id));
					else
						found = static_cast<TextComponent*>(TEXEngine::Core::UIManager::get().get_textbox(frame->_id));

					if ( found )
						lua_pushinteger(state,static_cast<lua_Integer>(found->get_horizontal_align()));
					else
						luaL_argerror(state,1,"The text component given was not found!");
				}
				else
					luaL_argerror(state,1,"The text component given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a text component argument!");


			return 1;
		};

		int	LuaUILibrary::_font_set_spacing( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::Font*	found = NULL;
					float					width_spacing = 0;
					float					height_spacing = 0;
					bool					width_active = false;
					bool					height_active = false;



					if ( argument_counter > 1 )
					{
						luaL_argcheck(state,lua_isnumber(state,2),2,"Width padding argument must be a float!");
						width_spacing = static_cast<float>(lua_tonumber(state,2));
						width_active = true;
					}

					if ( argument_counter > 2 )
					{
						luaL_argcheck(state,lua_isnumber(state,3),3,"Height argument must be a float!");
						height_spacing = static_cast<float>(lua_tonumber(state,3));
						height_active = true;
					}


					found = TEXEngine::Core::UIManager::get().get_font(frame->_id);

					if ( found )
					{
						if ( width_active )
							found->set_horizontal_shrinkage(width_spacing);

						if ( height_active )
							found->set_vertical_shrinkage(height_spacing);
					}
					else
						luaL_argerror(state,1,"The font given was not found!");
				}
				else
					luaL_argerror(state,1,"The font given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a font argument!");


			return 0;
		};

		int	LuaUILibrary::_font_get_id( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::Font*	found = TEXEngine::Core::UIManager::get().get_font(frame->_id);

										

					if ( found )
						lua_pushinteger(state,static_cast<lua_Integer>(found->get_id()));
					else
						luaL_argerror(state,1,"The font given was not found!");
				}
				else
					luaL_argerror(state,1,"The font given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a font argument!");


			return 1;
		};

		int	LuaUILibrary::_font_get_grid_size( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::Font*	found = TEXEngine::Core::UIManager::get().get_font(frame->_id);
					
					

					if ( found )
					{
						lua_pushinteger(state,static_cast<lua_Integer>(found->get_grid_width()));
						lua_pushinteger(state,static_cast<lua_Integer>(found->get_grid_height()));
					}
					else
						luaL_argerror(state,1,"The font given was not found!");
				}
				else
					luaL_argerror(state,1,"The font given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a font argument!");


			return 2;
		};

		int	LuaUILibrary::_font_get_spacing( lua_State* state )
		{
			int	argument_counter = lua_gettop(state);



			if ( argument_counter > 0 )
			{
				LuaUILibrary::_check_frame_function_args(state,1);
				LuaUIFrame*	frame = static_cast<LuaUIFrame*>(lua_touserdata(state,1));



				if ( frame )
				{
					TEXEngine::UI::Font*	found = TEXEngine::Core::UIManager::get().get_font(frame->_id);

										

					if ( found )
					{
						lua_pushnumber(state,static_cast<lua_Number>(found->get_horizontal_shrinkage()));
						lua_pushnumber(state,static_cast<lua_Number>(found->get_vertical_shrinkage()));
					}
					else
						luaL_argerror(state,1,"The font given was not found!");
				}
				else
					luaL_argerror(state,1,"The font given could not be verified!");
			}
			else
				luaL_argerror(state,1,"You must provide a font argument!");


			return 2;
		};


		std::string	LuaUILibrary::_to_lower( const std::string& input )
		{
			std::string				result(input.begin(),input.end());
			std::string::iterator	result_it = result.begin();
			char					difference = 'A' - 'a';



			for ( std::string::const_iterator it = input.begin();  it != input.end();  ++it )
			{
				if ( (*it) >= 'A'  &&  (*it) <= 'Z' )
					(*result_it) = (*it) - difference;
				else
					(*result_it) = (*it);

				++result_it;
			}


			return result;
		};


		LuaUILibrary::LuaUILibrary()
		{
		};

		LuaUILibrary::~LuaUILibrary()
		{
		};


		int	LuaUILibrary::init_library( lua_State* state )
		{
			luaL_newmetatable(state,"UI._frame_metatable");
			lua_pushstring(state,"__index");
			lua_pushvalue(state,-2);
			lua_settable(state,-3);
			for ( luaL_Reg* pointer = LuaUILibrary::_frame_functions;  pointer->func;  ++pointer )
			{
				lua_pushstring(state,pointer->name);
				lua_pushcfunction(state,pointer->func);
				lua_settable(state,-3);
			}


			luaL_newmetatable(state,"UI._button_metatable");
			lua_pushstring(state,"__index");
			lua_pushvalue(state,-2);
			lua_settable(state,-3);
			for ( luaL_Reg* pointer = LuaUILibrary::_frame_functions;  pointer->func;  ++pointer )
			{
				lua_pushstring(state,pointer->name);
				lua_pushcfunction(state,pointer->func);
				lua_settable(state,-3);
			}

			for ( luaL_Reg* pointer = LuaUILibrary::_button_functions;  pointer->func;  ++pointer )
			{
				lua_pushstring(state,pointer->name);
				lua_pushcfunction(state,pointer->func);
				lua_settable(state,-3);
			}


			luaL_newmetatable(state,"UI._checkbox_metatable");
			lua_pushstring(state,"__index");
			lua_pushvalue(state,-2);
			lua_settable(state,-3);
			for ( luaL_Reg* pointer = LuaUILibrary::_frame_functions;  pointer->func;  ++pointer )
			{
				lua_pushstring(state,pointer->name);
				lua_pushcfunction(state,pointer->func);
				lua_settable(state,-3);
			}

			for ( luaL_Reg* pointer = LuaUILibrary::_checkbox_functions;  pointer->func;  ++pointer )
			{
				lua_pushstring(state,pointer->name);
				lua_pushcfunction(state,pointer->func);
				lua_settable(state,-3);
			}


			luaL_newmetatable(state,"UI._container_metatable");
			lua_pushstring(state,"__index");
			lua_pushvalue(state,-2);
			lua_settable(state,-3);
			for ( luaL_Reg* pointer = LuaUILibrary::_frame_functions;  pointer->func;  ++pointer )
			{
				lua_pushstring(state,pointer->name);
				lua_pushcfunction(state,pointer->func);
				lua_settable(state,-3);
			}

			for ( luaL_Reg* pointer = LuaUILibrary::_container_functions;  pointer->func;  ++pointer )
			{
				lua_pushstring(state,pointer->name);
				lua_pushcfunction(state,pointer->func);
				lua_settable(state,-3);
			}

			luaL_newmetatable(state,"UI._image_metatable");
			lua_pushstring(state,"__index");
			lua_pushvalue(state,-2);
			lua_settable(state,-3);
			for ( luaL_Reg* pointer = LuaUILibrary::_frame_functions;  pointer->func;  ++pointer )
			{
				lua_pushstring(state,pointer->name);
				lua_pushcfunction(state,pointer->func);
				lua_settable(state,-3);
			}

			for ( luaL_Reg* pointer = LuaUILibrary::_image_functions;  pointer->func;  ++pointer )
			{
				lua_pushstring(state,pointer->name);
				lua_pushcfunction(state,pointer->func);
				lua_settable(state,-3);
			}


			luaL_newmetatable(state,"UI._label_metatable");
			lua_pushstring(state,"__index");
			lua_pushvalue(state,-2);
			lua_settable(state,-3);
			for ( luaL_Reg* pointer = LuaUILibrary::_frame_functions;  pointer->func;  ++pointer )
			{
				lua_pushstring(state,pointer->name);
				lua_pushcfunction(state,pointer->func);
				lua_settable(state,-3);
			}

			for ( luaL_Reg* pointer = LuaUILibrary::_text_component_functions;  pointer->func;  ++pointer )
			{
				lua_pushstring(state,pointer->name);
				lua_pushcfunction(state,pointer->func);
				lua_settable(state,-3);
			}


			luaL_newmetatable(state,"UI._slider_metatable");
			lua_pushstring(state,"__index");
			lua_pushvalue(state,-2);
			lua_settable(state,-3);
			for ( luaL_Reg* pointer = LuaUILibrary::_frame_functions;  pointer->func;  ++pointer )
			{
				lua_pushstring(state,pointer->name);
				lua_pushcfunction(state,pointer->func);
				lua_settable(state,-3);
			}

			for ( luaL_Reg* pointer = LuaUILibrary::_slider_functions;  pointer->func;  ++pointer )
			{
				lua_pushstring(state,pointer->name);
				lua_pushcfunction(state,pointer->func);
				lua_settable(state,-3);
			}


			luaL_newmetatable(state,"UI._textbox_metatable");
			lua_pushstring(state,"__index");
			lua_pushvalue(state,-2);
			lua_settable(state,-3);
			for ( luaL_Reg* pointer = LuaUILibrary::_frame_functions;  pointer->func;  ++pointer )
			{
				lua_pushstring(state,pointer->name);
				lua_pushcfunction(state,pointer->func);
				lua_settable(state,-3);
			}

			for ( luaL_Reg* pointer = LuaUILibrary::_textbox_functions;  pointer->func;  ++pointer )
			{
				lua_pushstring(state,pointer->name);
				lua_pushcfunction(state,pointer->func);
				lua_settable(state,-3);
			}

			for ( luaL_Reg* pointer = LuaUILibrary::_text_component_functions;  pointer->func;  ++pointer )
			{
				lua_pushstring(state,pointer->name);
				lua_pushcfunction(state,pointer->func);
				lua_settable(state,-3);
			}

			
			luaL_newmetatable(state,"UI._font_metatable");
			lua_pushstring(state,"__index");
			lua_pushvalue(state,-2);
			lua_settable(state,-3);
			for ( luaL_Reg* pointer = LuaUILibrary::_font_functions;  pointer->func;  ++pointer )
			{
				lua_pushstring(state,pointer->name);
				lua_pushcfunction(state,pointer->func);
				lua_settable(state,-3);
			}

			lua_newtable(state);
			lua_setglobal(state,"UI.__frame_on_event_functions");


			return 0;
		};

		int	LuaUILibrary::load_library( lua_State* state )
		{
			luaL_register(state,"UI",LuaUILibrary::_global_functions);
			lua_pushcfunction(state,&LuaUILibrary::init_library);
			lua_pcall(state,0,0,0);


			return 1;
		};

		std::vector<luaL_Reg>	LuaUILibrary::get_global_functions()
		{
			std::vector<luaL_Reg>	return_value;



			for ( luaL_Reg* pointer = LuaUILibrary::_global_functions;  pointer->func;  ++pointer )
				return_value.push_back(*pointer);


			return return_value;
		}

	}	/* Core */
}	/* TEXEngine */