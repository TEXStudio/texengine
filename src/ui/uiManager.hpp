#ifndef TEXENGINE_UIMANAGER_HPP_INCLUDED
#define TEXENGINE_UIMANAGER_HPP_INCLUDED

#include <vector>
#include <string>
#include <map>

#include "../renderer/iRenderer.hpp"
#include "uiUtility.hpp"
#include "../message/object.hpp"
#include "script/luaState.hpp"

//these can be removed once the render manager is linked with the ui manager
#include "../math/vector.hpp"
#include "../util/color.hpp"

namespace TEXEngine {
	
	namespace UI{
		class UIComponent;
		class Label;
		class Button;
		class Checkbox;
		class Container;
		class Slider;
		class Textbox;
		class Font;
		class UIImage;
	}//ui forward declarations

	namespace Core{

	class RenderManager; /* Forward declaration */
	
	using TEXEngine::Math::Vector2f;
	using TEXEngine::Util::ColorRGBAf;
	using namespace TEXEngine::UI;

class UIManager : public Object {
friend class RenderManager;
private:

	//statics
	static UIManager m_instance;

	static Container* m_root;
	
	LuaState m_state;

	//non static members
	std::map<ID, Button*> m_buttons;
	std::map<ID, Checkbox*> m_checkboxes;
	std::map<ID, Container*> m_containers;
	std::map<ID, UIImage*> m_images;
	std::map<ID, Label*> m_labels;
	std::map<ID, Slider*> m_sliders;
	std::map<ID, Textbox*> m_textboxes;
	std::map<ID, Font*> m_fonts;

	std::map<unsigned int, ID> m_navigation;
	std::map<ID, unsigned int> m_navigation2;//for performance, see note at find_component

	std::multimap<ZIndex, ID> m_containersTree;

	std::vector<std::string> m_lua_null_globals;
	std::map<std::string, bool> m_lua_bool_globals;
	std::map<std::string, int> m_lua_int_globals;
	std::map<std::string, unsigned int> m_lua_uint_globals;
	std::map<std::string, double> m_lua_double_globals;
	std::map<std::string, std::string> m_lua_string_globals;
	std::map<std::string, unsigned int> m_lua_event_globals;
	std::map<std::string, lua_CFunction> m_lua_function_globals;

	std::vector<std::string> m_lua_scripts;


	unsigned int m_currentTabIndex;
	unsigned int m_lastTabIndex;
	ID m_focusedComponent;
	UIComponent* m_focusedComponentPtr;//for performance (mainly used to avoid repeatedly calling get_component, which can potentially do a lot of looking up), see note at find_component

	IRenderer *m_renderer;

	unsigned long m_update_rate;

	bool m_initialised;
	bool m_reload_pending;

	//constructors	
	UIManager();

	//ui functions 
	//
	void transfer_focus(const ID newfocus);
	void transfer_focuss(UIComponent* newfocus);//for performance, see note at find_component
	void change_update_rate( const unsigned long code );
	void initialise(); //internal initialisation, mainly lua stuff
	void terminate();
	void light_terminate();
	void reload_ui();
	void purge_ui();
	void light_purge_ui();
	bool navigation_check(const ID, const unsigned int);


	//ASSUMPTION in order to guarantee id uniqueness: this function works and is called before creating any component. by "this function works"
	//I mean that it does its job of properly deleting the component that has the id, if any, and completely unbinding that component form anything 
	//it may be linked to
	void create_check(const ID id);
	UIComponent* get_component(const ID, const int) const;

	//purely lua-related functions
	//
	static int lua_create_root( lua_State* state );
	static int function_create_root( lua_State* state );
	bool add_script( const std::string* filename );
	void load_script( const std::string& filename );
	void run_script();
	void add_lua_global( std::string* name );
	void add_lua_global( std::string* name , bool value );
	void add_lua_global( std::string* name , int value );
	void add_lua_global( std::string* name , unsigned int value );
	void add_lua_global( std::string* name , double value );
	void add_lua_global( std::string* name , std::string* value );
	void add_lua_event_code( std::string* name , unsigned int code );
	void add_lua_function( std::string* name , lua_CFunction function );
	void remove_lua_global( std::string* name );
	void remove_lua_bool_global( std::string* name );
	void remove_lua_int_global( std::string* name );
	void remove_lua_uint_global( std::string* name );
	void remove_lua_double_global( std::string* name );
	void remove_lua_string_global( std::string* name );
	void remove_lua_event_code( std::string* name );
	void remove_lua_function( std::string* name );

protected:
	void setup_renderer(IRenderer *renderer);

public:

	static const ID MIN_ID = 0x100;

	static UIManager& get(){
		return m_instance;
	}

	LuaState& get_state(){
		return m_state;
	}

	//uicomponent related (create, access, ...) functions
	//
	
	//this function find a component that is in the navigation map and within the x,y given coordinates (which are in world space, so between -1 and 1)
	//this function is not particularly efficient however it is only used in the pc version but it's called everytime the mouse moves, which is potentially
	//a lot. this function calls get_component(ID), which is not particularly fast.
	//for these reasons, a few things have been put in place to accelerate this function at the cost of some memory
	UIComponent* find_component(const double x, const double y);
	//the main difference between the find_component and find_components functions are:
	//1) find_components returns several components, for instance if we have a container that contains a label and a button and they're all overlapping
	//   over x, y then they will all be returned
	//2) components returned by find_components don't have to be in the navigation map
	//TODO if we care: find_component could be refactored into find_components by adding 2 flags to it: bool checkNavigable, unsigned int limit. then the current calls to find_component 
	//     could be replaced by (pseudocode for clarity) find_components(x, y, checkVisibility = true, checkNavigable=true, limit=1);
	//     the current find_components call would be changed to find_components(x, y, visibility, false, 0)
	std::vector<UIComponent*> find_components(const double x, const double y, const bool checkVisibility = true);
	void attach_to_root(const ID id, const ZIndex z = 0);
	//adds the tabindex for the given component to the navigation map iif tabindex > 0. if tabindex == 0 then every reference to the 
	//given component will be cleared from the navigation map
	//note that the tabindex is a global thing and not a "per-screen" thing (since we don't have a 'screen' concept)
	//so each tabindex can only be mapped to once however it is possible to map multiple tabindices to the same component
	void add_tabindex(const unsigned int tabindex, const ID component);
	//if the ID returned is < 0x100, something went wrong OR we're talking about the root container
	void create_label(const ID id, const ID font, const char* text, const float x = 0.0f, const float y = 0.0f, const float w = 0.0f, const float h = 0.0f);
	void create_button(const ID id, const float x = 0.0f, const float y = 0.0f, const float w = 0.0f, const float h = 0.0f);
	void create_checkbox(const ID id, const float x = 0.0f, const float y = 0.0f, const float w = 0.0f, const float h = 0.0f);
	void create_textbox(const ID id, const ID font, const char* text, const float x = 0.0f, const float y = 0.0f, const float w = 0.0f, const float h = 0.0f);
	void create_slider(const ID id, const int step = 50, const int min = 0, const int max = 100, const float x = 0.0f, const float y = 0.0f, const float w = 0.0f, const float h = 0.0f);
	void create_image(const ID id, const unsigned int texid, const float x = 0.0f, const float y = 0.0f, const float w = 0.0f, const float h = 0.0f);
	void create_image(const ID id, const unsigned int texid, const float u1, const float v1, const float u2, const float v2, const float u3, const float v3, const float u4, const float v4, const float x = 0.0f, const float y = 0.0f, const float w = 0.0f, const float h = 0.0f);
	void create_container(const ID id, const ZIndex z=0, const float x = 0.0f, const float y = 0.0f, const float w = 0.0f, const float h = 0.0f);	
	void create_font(const ID id);
	//this is only for containers that are directly attached to m_root. to reset the z of a component within a container, detach it and re attach it with the new z
	void set_container_z(const ID container_id, const ZIndex newZ);
	Label* get_label(const ID) const;
	Button* get_button(const ID) const;
	Checkbox* get_checkbox(const ID) const;
	Textbox* get_textbox(const ID) const;
	Slider* get_slider(const ID) const;
	UIImage* get_image(const ID) const;
	Container* get_container(const ID) const;
	UIComponent* get_component(const ID) const;
	Font* get_font(const ID) const;

	//rendering related functions
	void render_box(float x, float y, float w, float h, 
		const ColorRGBAf &col, int texid = 0,
		const Vector2f &uv1 = Vector2f(0,0), const Vector2f &uv2 = Vector2f(0,0), 
		const Vector2f &uv3 = Vector2f(0,0), const Vector2f &uv4 = Vector2f(0,0));
	void render_ui();
	void aspect_modifier(const float mod) const;
	float aspect_modifier() const;
	TEXEngine::Math::Vector2f unproject(const int x, const int y) const;

	//all other functions
	//
	//this must be called by user code because this function registers a bunch of events. this is necessary because we can't just do that in the constructor for instance because the messaging system may not exist yet
	void setup(); 
	void on_event(const unsigned long event_code, const Message& parameters);
	//returns true if the event has been published to lua
	bool is_event_available( const unsigned int code ) const;

	~UIManager();

};

	}//namespace core
}//namespace TEXEngine

#endif /* TEXENGINE_UIMANAGER_HPP_INCLUDED */