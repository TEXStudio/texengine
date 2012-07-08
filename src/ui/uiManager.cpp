#include <cmath>

#include "uiManager.hpp"
#include "uiComponent.hpp"
#include "label.hpp"
#include "button.hpp"
#include "checkbox.hpp"
#include "container.hpp"
#include "slider.hpp"
#include "textbox.hpp"
#include "font.hpp"
#include "uiImage.hpp"

#include "script/luaUILibrary.hpp"
#include "script/luaShrunkDefaultLibraries.hpp"
#include "../message/TEXEngineInterface.hpp"
#include "../log/logmanager.hpp"
#include "../math/vector.hpp"
#include "../math/aabb.hpp"


using namespace TEXEngine::UI;
using namespace TEXEngine::Math;
using namespace std;

namespace TEXEngine{
	namespace Core{

Container* UIManager::m_root = new (std::nothrow) Container(1, -1.0f, -1.0f, 2.0f, 2.0f);

UIManager UIManager::m_instance;

UIManager::UIManager() 
	: Object(5), m_currentTabIndex(0), m_lastTabIndex(0), m_focusedComponent(m_root->get_id()), m_focusedComponentPtr(m_root), m_update_rate(UPDATE),
	  m_initialised(false) , m_reload_pending(false), m_renderer(NULL){
	//root has focus
}

void UIManager::transfer_focuss(UIComponent* newfocus){
	if(newfocus && m_focusedComponent != newfocus->get_id()/* && newfocus->is_visible()*/){ //visibility check is performed before calling this function
		if(m_focusedComponent > 0 && m_focusedComponentPtr){
			m_focusedComponentPtr->system_behaviour(COMPONENT_ON_FOCUS_LOST);
			m_focusedComponentPtr->on_event(m_state, COMPONENT_ON_FOCUS_LOST, Message());
		}
		m_focusedComponent = newfocus->get_id();
		m_focusedComponentPtr = newfocus;
		m_focusedComponentPtr->system_behaviour(COMPONENT_ON_FOCUS_GAINED);
		m_focusedComponentPtr->on_event(m_state, COMPONENT_ON_FOCUS_GAINED, Message());
	}
}

void UIManager::transfer_focus(const ID id){
	UIComponent* newFocus = get_component(id);
	transfer_focuss(newFocus);
}

void UIManager::change_update_rate( const unsigned long code ){
	if ( code != m_update_rate ){
		if ( code >= UPDATE  &&  code <= UPDATE_SLOW ){
			unregister_event(m_update_rate);
			m_update_rate = code;
			register_event(m_update_rate);
		}
	}
}

void UIManager::initialise(){
	if ( !m_initialised ){
		std::vector<luaL_Reg> funcs = LuaUILibrary::get_global_functions();

		//m_state.set_error_function(error_func);
		m_state.create();
		//m_state.load_lib_base();
		m_state.load_lib_table();
		//m_state.load_lib_IO();
		//m_state.load_lib_OS();
		m_state.load_lib_string();
		m_state.load_lib_math();
		//m_state.load_lib_debug();
		//m_state.load_lib_package();

		m_state.load_library(std::string(""),LuaShrunkDefaultLibraries::load_library);
		m_state.load_variable(std::string("_VERSION"),std::string("TEXEngine UI v1.00"));
		m_state.run_lua_function(LuaUILibrary::init_library);

		for ( std::vector<luaL_Reg>::iterator it = funcs.begin();  it != funcs.end();  ++it )
			m_state.load_variable((*it));

		for ( map<string,lua_CFunction>::const_iterator it = m_lua_function_globals.begin();  it != m_lua_function_globals.end();  ++it )
			m_state.load_variable(it->first,it->second);

		for ( vector<string>::const_iterator it = m_lua_null_globals.begin();  it != m_lua_null_globals.end();  ++it )
			m_state.load_variable((*it));

		for ( map<string,bool>::const_iterator it = m_lua_bool_globals.begin();  it != m_lua_bool_globals.end();  ++it )
			m_state.load_variable(it->first,it->second);

		for ( map<string,int>::const_iterator it = m_lua_int_globals.begin();  it != m_lua_int_globals.end();  ++it )
			m_state.load_variable(it->first,it->second);
	
		for ( map<string,unsigned int>::const_iterator it = m_lua_uint_globals.begin();  it != m_lua_uint_globals.end();  ++it )
			m_state.load_variable(it->first,it->second);

		for ( map<string,double>::const_iterator it = m_lua_double_globals.begin();  it != m_lua_double_globals.end();  ++it )
			m_state.load_variable(it->first,it->second);

		for ( map<string,string>::const_iterator it = m_lua_string_globals.begin();  it != m_lua_string_globals.end();  ++it )
			m_state.load_variable(it->first,it->second);

		for ( map<string,unsigned int>::const_iterator it = m_lua_event_globals.begin();  it != m_lua_event_globals.end();  ++it ) {
			m_state.load_variable(it->first,it->second);
			register_event(it->second);
		}

		m_state.run_function(function_create_root);


		m_initialised = true;
	}
}

void UIManager::terminate(){
	if ( m_initialised ){
		purge_ui();

		delete m_root;
		m_initialised = false;
	}
}

void UIManager::light_terminate(){
	if ( m_initialised ){
		light_purge_ui();

		delete m_root;
		m_initialised = false;
	}
}

void UIManager::reload_ui(){
	purge_ui();
	m_initialised = false;
	initialise();

	for ( vector<string>::const_iterator it = m_lua_scripts.begin();  it != m_lua_scripts.end();  ++it ){
		load_script(*it);
		run_script();
	}
}

void UIManager::purge_ui(){
	light_purge_ui();

	for ( map<string,unsigned int>::const_iterator it = m_lua_event_globals.begin();  it != m_lua_event_globals.end();  ++it )
		unregister_event(it->second);
}

void UIManager::light_purge_ui(){
	for(map<ID, Button*>::const_iterator it = m_buttons.begin(); it!= m_buttons.end(); ++it){
		delete (*it).second;
	}
	m_buttons.clear();

	for(map<ID, Checkbox*>::const_iterator it = m_checkboxes.begin(); it!= m_checkboxes.end(); ++it){
		delete (*it).second;
	}
	m_checkboxes.clear();

	for(map<ID, Container*>::const_iterator it = m_containers.begin(); it!= m_containers.end(); ++it){
		delete (*it).second;
	}
	m_containers.clear();

	for(map<ID, UIImage*>::const_iterator it = m_images.begin(); it!= m_images.end(); ++it){
		delete (*it).second;
	}
	m_images.clear();

	for(map<ID, Label*>::const_iterator it = m_labels.begin(); it!= m_labels.end(); ++it){
		delete (*it).second;
	}
	m_labels.clear();

	for(map<ID, Slider*>::const_iterator it = m_sliders.begin(); it!= m_sliders.end(); ++it){
		delete (*it).second;
	}
	m_sliders.clear();

	for(map<ID, Textbox*>::const_iterator it = m_textboxes.begin(); it!= m_textboxes.end(); ++it){
		delete (*it).second;
	}
	m_textboxes.clear();

	for(map<ID, Font*>::const_iterator it = m_fonts.begin(); it != m_fonts.end(); ++it)
		delete (*it).second;
	m_fonts.clear();

	m_containersTree.clear();

	m_navigation.clear();
	m_navigation2.clear();

	m_currentTabIndex = 0;
	m_lastTabIndex = 0;
	m_focusedComponent = m_root->get_id();
	m_focusedComponentPtr = m_root;
		
	m_state.destroy();
}

bool UIManager::navigation_check(const ID id, const unsigned int tab){
	UIComponent* c = get_component(id);
	if(c && c->is_enabled() && m_currentTabIndex != tab && c->is_visible()){
		m_lastTabIndex = m_currentTabIndex;
		m_currentTabIndex = tab;
		transfer_focuss(c);
		return true;
	}
	return false;
}

void UIManager::create_check(const ID id){
	
	UIComponent* check = get_component(id);
	if(check){ //if there exists a component with that id
		Container* c = get_container(check->get_parent());
		if(c){//if that component has a parent
			c->detach(id);
		}

		//now to find out what kind of component we're dealing with...
		switch(check->get_type()){
			case UIComponent::COMPONENT_TYPE_BUTTON:{
				map<ID, Button*>::iterator it = m_buttons.find(id);
				delete (*it).second;
				m_buttons.erase(it);
				}
				break;
			case UIComponent::COMPONENT_TYPE_CHECKBOX:{
				map<ID, Checkbox*>::iterator it = m_checkboxes.find(id);
				delete (*it).second;
				m_checkboxes.erase(it);
				}
				break;
			case UIComponent::COMPONENT_TYPE_CONTAINER:{
				map<ID, Container*>::iterator it = m_containers.find(id);
				delete (*it).second;
				m_containers.erase(it);
				}
				break;
			case UIComponent::COMPONENT_TYPE_IMAGE:{
				map<ID, UIImage*>::iterator it = m_images.find(id);
				delete (*it).second;
				m_images.erase(it);
				}
				break;
			case UIComponent::COMPONENT_TYPE_LABEL:{
				map<ID, Label*>::iterator it = m_labels.find(id);
				delete (*it).second;
				m_labels.erase(it);
				}
				break;
			case UIComponent::COMPONENT_TYPE_SLIDER:{
				map<ID, Slider*>::iterator it = m_sliders.find(id);
				delete (*it).second;
				m_sliders.erase(it);
				}
				break;
			case UIComponent::COMPONENT_TYPE_TEXTBOX:{
				map<ID, Textbox*>::iterator it = m_textboxes.find(id);
				delete (*it).second;
				m_textboxes.erase(it);
				}
				break;
		}
	}
}

UIComponent* UIManager::get_component(const ID id, const int type) const{
	map<ID, Button*>::const_iterator button_it;
	map<ID, Checkbox*>::const_iterator checkbox_it;
	map<ID, Container*>::const_iterator container_it;
	map<ID, UIImage*>::const_iterator image_it;
	map<ID, Label*>::const_iterator label_it;
	map<ID, Slider*>::const_iterator slider_it;
	map<ID, Textbox*>::const_iterator textbox_it;
	switch(type){
		case UIComponent::COMPONENT_TYPE_BUTTON:{
			map<ID, Button*>::const_iterator it = m_buttons.find(id);			
			if(it != m_buttons.end())
				return it->second;
			}
			break;
		case UIComponent::COMPONENT_TYPE_CHECKBOX:{
			map<ID, Checkbox*>::const_iterator it = m_checkboxes.find(id);
			if(it != m_checkboxes.end())
				return it->second;
			}
			break;
		case UIComponent::COMPONENT_TYPE_CONTAINER:{
			if(id == m_root->get_id())
				return m_root;
			map<ID, Container*>::const_iterator it = m_containers.find(id);
			if(it != m_containers.end())
				return it->second;
			}
			break;
		case UIComponent::COMPONENT_TYPE_IMAGE:{
			map<ID, UIImage*>::const_iterator it = m_images.find(id);
			if(it != m_images.end())
				return it->second;
			}
			break;
		case UIComponent::COMPONENT_TYPE_LABEL:{
			map<ID, Label*>::const_iterator it = m_labels.find(id);
			if(it != m_labels.end())
				return it->second;
			}
			break;
		case UIComponent::COMPONENT_TYPE_SLIDER:{
			map<ID, Slider*>::const_iterator it = m_sliders.find(id);
			if(it != m_sliders.end())
				return it->second;
			}
			break;
		case UIComponent::COMPONENT_TYPE_TEXTBOX:{
			map<ID, Textbox*>::const_iterator it = m_textboxes.find(id);
			if(it != m_textboxes.end())
				return it->second;
			}
	}
	return NULL;
}

int UIManager::lua_create_root(lua_State* state){
	LuaUIFrame* frame = static_cast<LuaUIFrame*>(lua_newuserdata(state, sizeof(LuaUIFrame)));

	if(frame){
		luaL_getmetatable(state,"UI._container_metatable");
		lua_setmetatable(state,-2);
		frame->_id = m_root->get_id();
		memset(frame->_type,'\0',sizeof(frame->_type));
		memcpy(frame->_type,"container",( sizeof(frame->_type) < sizeof("container")  ?  sizeof(frame->_type) : sizeof("container") ));
		lua_setglobal(state,"UIRoot");
	}

	return 0;
}

int UIManager::function_create_root( lua_State* state ) {
	if(state){
		lua_pushcfunction(state,UIManager::lua_create_root);
		lua_pcall(state,0,0,0);
	}

	return 0;
}

bool UIManager::add_script( const string* filename ){
	bool return_value = false;

	if ( filename ){
		string name(*filename);
		vector<string>::const_iterator it = m_lua_scripts.begin();
		bool found = false;

		while( !found && it != m_lua_scripts.end() ){
			if ( (*it) == name )
				found = true;
			else
				++it;
		}

		if ( !found ){
			m_lua_scripts.push_back(name);
			load_script(name);
			return_value = true;
		}
	}

	return return_value;
}

void UIManager::load_script( const string& filename ){
	m_state.load_script_file(filename);
}

void UIManager::run_script(){
	if ( m_state.run_script() != 0)
		LogManager::get().log_error(m_state.get_stack().c_str());
}

void UIManager::add_lua_global( std::string* name ){
	if ( name ){
		vector<string>::iterator it = m_lua_null_globals.begin();

		if ( it != m_lua_null_globals.end() ){
			bool found = false;

			while( !found  &&  it != m_lua_null_globals.end() ){
				if ( (*it) == *name )
					found = true;
				else
					++it;
			}

			if ( !found )
				m_lua_null_globals.push_back(*name);
		}
		else
			m_lua_null_globals.push_back(*name);
	}
}

void UIManager::add_lua_global( std::string* name , bool value ){
	if ( name ){
		map<string,bool>::iterator it = m_lua_bool_globals.find(*name);

		if ( it != m_lua_bool_globals.end() )
			it->second = value;
		else
			m_lua_bool_globals.insert(pair<string,bool>(*name,value));
	}
}

void UIManager::add_lua_global( std::string* name , int value ){
	if ( name ){
		map<string,int>::iterator it = m_lua_int_globals.find(*name);

		if ( it != m_lua_int_globals.end() )
			it->second = value;
		else
			m_lua_int_globals.insert(pair<string,int>(*name,value));
	}
}

void UIManager::add_lua_global( std::string* name , unsigned int value ){
	if ( name ){
		map<string,unsigned int>::iterator it = m_lua_uint_globals.find(*name);

		if ( it != m_lua_uint_globals.end() )
			it->second = value;
		else
			m_lua_uint_globals.insert(pair<string,unsigned int>(*name,value));
	}
}

void UIManager::add_lua_global( std::string* name , double value ){
	if ( name ){
		map<string,double>::iterator it = m_lua_double_globals.find(*name);

		if ( it != m_lua_double_globals.end() )
			it->second = value;
		else
			m_lua_double_globals.insert(pair<string,double>(*name,value));
	}
}

void UIManager::add_lua_global( std::string* name , std::string* value ){
	if ( name  &&  value ){
		map<string,string>::iterator it = m_lua_string_globals.find(*name);

		if ( it != m_lua_string_globals.end() )
			it->second = *value;
		else
			m_lua_string_globals.insert(pair<string,string>(*name,*value));
	}
}

void UIManager::add_lua_event_code( std::string* name , unsigned int code ){
	if ( name ){
		map<string,unsigned int>::iterator it = m_lua_event_globals.find(*name);

		if ( it != m_lua_event_globals.end() )
			it->second = code;
		else
			m_lua_event_globals.insert(pair<string,unsigned int>(*name,code));
	}
}

void UIManager::add_lua_function( std::string* name , lua_CFunction function ){
	if ( name  &&  function ){
		map<string,lua_CFunction>::iterator it = m_lua_function_globals.find(*name);

		if ( it!= m_lua_function_globals.end() )
			it->second = function;
		else
			m_lua_function_globals.insert(pair<string,lua_CFunction>(*name,function));
	}
}

void UIManager::remove_lua_global( std::string* name ){
	if ( name ){
		vector<string>::iterator it = m_lua_null_globals.begin();

		if ( it != m_lua_null_globals.end() ){
			bool found = false;

			while( !found  &&  it != m_lua_null_globals.end() ){
				if ( (*it) == *name ){
					found = true;
					m_lua_null_globals.erase(it);
				}
				else
					++it;
			}
		}
	}
}

void UIManager::remove_lua_bool_global( std::string* name ){
	if ( name ){
		map<string,bool>::iterator it = m_lua_bool_globals.find(*name);

		if ( it != m_lua_bool_globals.end() )
			m_lua_bool_globals.erase(it);
	}
}

void UIManager::remove_lua_int_global( std::string* name ){
	if ( name ){
		map<string,int>::iterator it = m_lua_int_globals.find(*name);

		if ( it != m_lua_int_globals.end() )
			m_lua_int_globals.erase(it);
	}
}

void UIManager::remove_lua_uint_global( std::string* name ){
	if ( name ){
		map<string,unsigned int>::iterator it = m_lua_uint_globals.find(*name);

		if ( it != m_lua_uint_globals.end() )
			m_lua_uint_globals.erase(it);
	}
}

void UIManager::remove_lua_double_global( std::string* name ){
	if ( name ){
		map<string,double>::iterator it = m_lua_double_globals.find(*name);

		if ( it != m_lua_double_globals.end() )
			m_lua_double_globals.erase(it);
	}
}

void UIManager::remove_lua_string_global( std::string* name ){
	if ( name ){
		map<string,string>::iterator it = m_lua_string_globals.find(*name);

		if ( it != m_lua_string_globals.end() )
			m_lua_string_globals.erase(it);
	}
}

void UIManager::remove_lua_event_code( std::string* name ){
	if ( name ){
		map<string,unsigned int>::iterator it = m_lua_event_globals.find(*name);

		if ( it != m_lua_event_globals.end() )
			m_lua_event_globals.erase(it);
	}
}

void UIManager::remove_lua_function( std::string* name ){
	if ( name ){
		map<string,lua_CFunction>::iterator it = m_lua_function_globals.find(*name);

		if ( it != m_lua_function_globals.end() )
			m_lua_function_globals.erase(it);
	}
}

UIComponent* UIManager::find_component(const double x, const double y){
	Vector2f screenPos(x,y);
	for(multimap<ZIndex, ID>::reverse_iterator i = m_containersTree.rbegin(); i != m_containersTree.rend(); ++i){
		Container* c = get_container(i->second);
		BoundingBox2 b(Vector2f(c->get_x(), c->get_y()), Vector2f(c->get_x()+c->get_width(), c->get_y()+c->get_height()));
		if(b.contains(screenPos)){
			for(multimap<ZIndex, ID>::reverse_iterator j = c->m_components.rbegin(); j != c->m_components.rend(); ++j){
				UIComponent* comp = get_component(j->second);
				if(!comp)
					continue;
				if(!comp->is_visible())
					continue;
				if(m_navigation2.find(comp->get_id()) == m_navigation2.end())
					continue;
				float cx = comp->get_x();
				float cy = comp->get_y();
				BoundingBox2 d(Vector2f(cx, cy), Vector2f(cx + comp->get_width(), cy + comp->get_height()));
				if(d.contains(screenPos)){
					return comp;
				}
			}
		}
	}
	return NULL;
}

vector<UIComponent*> UIManager::find_components(const double x, const double y, const bool checkVisibility){
	vector<UIComponent*> result;
	Vector2f screenPos(x,y);
	for(multimap<ZIndex, ID>::reverse_iterator i = m_containersTree.rbegin(); i != m_containersTree.rend(); ++i){
		Container* c = get_container(i->second);
		BoundingBox2 b(Vector2f(c->get_x(), c->get_y()), Vector2f(c->get_x()+c->get_width(), c->get_y()+c->get_height()));
		if(b.contains(screenPos)){
			for(multimap<ZIndex, ID>::reverse_iterator j = c->m_components.rbegin(); j != c->m_components.rend(); ++j){
				UIComponent* comp = get_component(j->second);
				if(!comp)
					continue;
				if(checkVisibility && !comp->is_visible())
					continue;
				float cx = comp->get_x();
				float cy = comp->get_y();
				BoundingBox2 d(Vector2f(cx, cy), Vector2f(cx+comp->get_width(), cy+comp->get_height()));
				if(d.contains(screenPos)){
					result.push_back(comp);
				}
			}
		}
	}
	return result;
}

void UIManager::attach_to_root(const ID id, const ZIndex z){
	if(m_root){
		m_root->attach(id, z);
	}
}

void UIManager::add_tabindex(const unsigned int tabindex, const ID component){
	if(tabindex > 0){
		//by default, the root container will have the focus (which is essentially the same thing as if no component had the focus)
		//so if we add tabindex that means we're interested in having focused components and thus let's try to add the first one
		if(m_currentTabIndex == 0){
			transfer_focus(component);
			if(m_focusedComponent == component){
				m_lastTabIndex = m_currentTabIndex;
				m_currentTabIndex = tabindex;
				UIComponent* c = get_component(component);
			}
		}
		map<unsigned int, ID>::iterator it;
		if((it = m_navigation.find(tabindex)) != m_navigation.end()){
			m_navigation2.erase(it->second);
			m_navigation.erase(it);
		}
		map<ID, unsigned int>::iterator it2;
		if((it2 = m_navigation2.find(component)) != m_navigation2.end()){
			m_navigation.erase(it2->second);
			m_navigation2.erase(it2);
		}

		m_navigation[tabindex] = component;
		m_navigation2[component] = tabindex;
	}else{
		map<ID, unsigned int>::iterator it2;
		//find which to remove
		if((it2 = m_navigation2.find(component)) != m_navigation2.end()){
			m_navigation.erase(it2->second);
			m_navigation2.erase(it2);
		}
	}
}

void UIManager::create_label(const ID id, const ID font, const char* text, const float x, const float y, const float w, const float h) {
	if(id < MIN_ID)
		return;
	create_check(id);

	Label* label = new (std::nothrow) Label(id, font, text);
	if(label){
		label->set_x(x);
		label->set_y(y);
		label->set_width(w);
		label->set_height(h);
		m_labels[id] = label;
	}
}

void UIManager::create_button(const ID id, const float x, const float y, const float w, const float h){
	if(id < MIN_ID)
		return;
	create_check(id);

	Button* button = new (std::nothrow) Button(id);
	if(button){
		button->set_x(x);
		button->set_y(y);
		button->set_width(w);
		button->set_height(h);
		m_buttons[id] = button;
	}
}

void UIManager::create_checkbox(const ID id, const float x, const float y, const float w, const float h){
	if(id < MIN_ID)
		return;
	create_check(id);

	Checkbox* checkbox = new (std::nothrow) Checkbox(id);
	if(checkbox){
		checkbox->set_x(x);
		checkbox->set_y(y);
		checkbox->set_width(w);
		checkbox->set_height(h);
		m_checkboxes[id] = checkbox;
	}
}

void UIManager::create_textbox(const ID id, const ID font, const char* text, const float x, const float y, const float w, const float h){
	if(id < MIN_ID)
		return;
	create_check(id);

	Textbox* textbox = new (std::nothrow) Textbox(id, font, text);
	if(textbox){
		textbox->set_x(x);
		textbox->set_y(y);
		textbox->set_width(w);
		textbox->set_height(h);
		m_textboxes[id] = textbox;
	}
}

void UIManager::create_slider(const ID id, const int step, const int min, const int max, const float x, const float y, const float w, const float h){
	if(id < MIN_ID)
		return;
	create_check(id);

	Slider* slider = new (std::nothrow) Slider(id, step, min, max);
	if(slider){
		slider->set_x(x);
		slider->set_y(y);
		slider->set_width(w);
		slider->set_height(h);
		m_sliders[id] = slider;
	}
}

void UIManager::create_image(const ID id, const unsigned int texid, const float x, const float y, const float w, const float h){
	if(id < MIN_ID)
		return;
	create_check(id);

	UIImage* img = new (std::nothrow) UIImage(id, texid);
	if(img){
		img->set_x(x);
		img->set_y(y);
		img->set_width(w);
		img->set_height(h);
		m_images[id] = img;	
	}
}

void UIManager::create_image(const ID id, const unsigned int texid, const float u1, const float v1, const float u2, const float v2, const float u3, const float v3, const float u4, const float v4, const float x, const float y, const float w, const float h){
	if(id < MIN_ID)
		return;
	create_check(id);

	UIImage* img = new (std::nothrow) UIImage(id, texid, u1, v1, u2, v2, u3, v3, u4, v4);
	if(img){
		img->set_x(x);
		img->set_y(y);
		img->set_width(w);
		img->set_height(h);
		m_images[id] = img;	
	}

}

void UIManager::create_container(const ID id, const ZIndex z, const float x, const float y, const float w, const float h){
	if(id < MIN_ID)
		return;
	create_check(id);

	Container* container = new (std::nothrow) Container(id);
	if(container){
		container->set_x(x);
		container->set_y(y);
		container->set_width(w);
		container->set_height(h);
		m_containers[id] = container;
		m_containersTree.insert(pair<ZIndex, ID>(z, id));
	}
}

void UIManager::create_font(const ID id){
	Font* check = get_font(id);
	if(check){
		map<ID, Font*>::iterator it = m_fonts.find(id);
		delete check;
		m_fonts.erase(it);
	}

	Font* f = new (std::nothrow) Font(id);
	if(f)
		m_fonts[id] = f;
}

void UIManager::set_container_z(const ID container_id, const ZIndex newZ){
	//since root is not in m_containersTree, it cannot be affected (i.e. we can't "accidentally" delete root)
	for(std::multimap<ZIndex, ID>::iterator it = m_containersTree.begin(); it != m_containersTree.end(); ++it){
		if((*it).second == container_id){
			m_containersTree.erase(it);
			break;
		}
	}
	m_containersTree.insert(std::pair<ZIndex, ID>(newZ, container_id));
}

Label* UIManager::get_label(const ID id) const{
	map<ID, Label*>::const_iterator i = m_labels.find(id);
	if(i != m_labels.end()){
		return i->second;
	}
	return NULL;
}

Button* UIManager::get_button(const ID id) const{
	map<ID, Button*>::const_iterator i = m_buttons.find(id);
	if(i != m_buttons.end()) {
		return i->second;
	}
	return NULL;
}

Checkbox* UIManager::get_checkbox(const ID id) const{
	map<ID, Checkbox*>::const_iterator i = m_checkboxes.find(id);
	if(i != m_checkboxes.end()) {
		return i->second;
	}
	return NULL;
}

Textbox* UIManager::get_textbox(const ID id) const{
	map<ID, Textbox*>::const_iterator i = m_textboxes.find(id);
	if(i != m_textboxes.end()) {
		return i->second;
	}
	return NULL;
}

Slider* UIManager::get_slider(const ID id) const{
	map<ID, Slider*>::const_iterator i = m_sliders.find(id);
	if(i != m_sliders.end()) {
		return i->second;
	}
	return NULL;
}

UIImage* UIManager::get_image(const ID id) const{
	map<ID, UIImage*>::const_iterator i = m_images.find(id);
	if(i != m_images.end()) {
		return i->second;
	}
	return NULL;
}

Container* UIManager::get_container(const ID id) const{
	if(id == m_root->get_id())
		return m_root;

	map<ID, Container*>::const_iterator i = m_containers.find(id);
	if(i != m_containers.end()) {
		return i->second;
	}

	return NULL;
}

UIComponent* UIManager::get_component(const ID id) const{
	map<ID, Button*>::const_iterator button_it;
	map<ID, Checkbox*>::const_iterator checkbox_it;
	map<ID, Container*>::const_iterator container_it;
	map<ID, UIImage*>::const_iterator image_it;
	map<ID, Label*>::const_iterator label_it;
	map<ID, Slider*>::const_iterator slider_it;
	map<ID, Textbox*>::const_iterator textbox_it;

	if(id == m_root->get_id())
		return m_root;

	if((button_it = m_buttons.find(id)) != m_buttons.end())
		return button_it->second;		
	if((checkbox_it = m_checkboxes.find(id)) != m_checkboxes.end())
		return checkbox_it->second;		
	if((container_it = m_containers.find(id)) != m_containers.end())
		return container_it->second;		
	if((image_it = m_images.find(id)) != m_images.end())
		return image_it->second;		
	if((label_it = m_labels.find(id)) != m_labels.end())
		return label_it->second;		
	if((slider_it = m_sliders.find(id)) != m_sliders.end())
		return slider_it->second;		
	if((textbox_it = m_textboxes.find(id)) != m_textboxes.end()) 
		return textbox_it->second;
	return NULL;
}

Font* UIManager::get_font(const ID id) const{
	map<ID, Font*>::const_iterator i = m_fonts.find(id);
	if(i != m_fonts.end()) {
		return i->second;
	}
	return NULL;
}

void UIManager::render_ui(){
	if(m_root)
		m_root->render();
}

void UIManager::render_box(float x, float y, float w, float h, 
			const ColorRGBAf &col, int texid,
			const Vector2f &uv1, const Vector2f &uv2, 
			const Vector2f &uv3, const Vector2f &uv4)
{
	if(m_renderer)
		m_renderer->render_box(x, y, w, h, col, texid, uv1, uv2, uv3, uv4);
}

void UIManager::aspect_modifier(const float mod) const
{
	if(m_renderer)
		m_renderer->aspect_modifier_2d(mod);
}

float UIManager::aspect_modifier() const{
	if(m_renderer)
		return m_renderer->aspect_modifier_2d();
	return 1.0f;
}

Vector2f UIManager::unproject(const int x, const int y) const{
	if(m_renderer)
		return m_renderer->map_2d(x, y);
	return Vector2f();
}

void UIManager::setup(){
	register_event(INITIALISE);
	register_event(EXIT);
	register_event(UIMANAGER_CHANGE_UPDATE_RATE);
	register_event(m_update_rate);
	register_event(RELOAD_UI);
	register_event(LOAD_SCRIPT);
	register_event(RUN_SCRIPT);
	register_event(LOAD_AND_RUN_SCRIPT);
	register_event(ADD_NULL_VARIABLE);
	register_event(ADD_BOOL_VARIABLE);
	register_event(ADD_INT_VARIABLE);
	register_event(ADD_UINT_VARIABLE);
	register_event(ADD_DOUBLE_VARIABLE);
	register_event(ADD_STRING_VARIABLE);
	register_event(ADD_EVENT_VARIABLE );
	register_event(ADD_FUNCTION_VARIABLE );
	register_event(REMOVE_NULL_VARIABLE);
	register_event(REMOVE_BOOL_VARIABLE);
	register_event(REMOVE_INT_VARIABLE);
	register_event(REMOVE_UINT_VARIABLE);
	register_event(REMOVE_DOUBLE_VARIABLE);
	register_event(REMOVE_STRING_VARIABLE);
	register_event(REMOVE_EVENT_VARIABLE);
	register_event(REMOVE_FUNCTION_VARIABLE);


	//publish ui-specific events as globals
	string s("COMPONENT_ON_MOUSE_HOVER");
	add_lua_global(&s, static_cast<unsigned int>(COMPONENT_ON_MOUSE_HOVER));
	s = "COMPONENT_ON_MOUSE_OUT";
	add_lua_global(&s, static_cast<unsigned int>(COMPONENT_ON_MOUSE_OUT));
	s = "COMPONENT_ON_FOCUS_GAINED";
	add_lua_global(&s, static_cast<unsigned int>(COMPONENT_ON_FOCUS_GAINED));
	s = "COMPONENT_ON_FOCUS_LOST";
	add_lua_global(&s, static_cast<unsigned int>(COMPONENT_ON_FOCUS_LOST));
	s = "COMPONENT_ON_VALUE_CHANGED";
	add_lua_global(&s, static_cast<unsigned int>(COMPONENT_ON_VALUE_CHANGED));
	s = "COMPONENT_ACTION_PERFORMED";
	add_lua_global(&s, static_cast<unsigned int>(COMPONENT_ACTION_PERFORMED));
	s = "COMPONENT_ACTION_LEFT";
	add_lua_global(&s, static_cast<unsigned int>(COMPONENT_ACTION_LEFT));
	s = "COMPONENT_ACTION_RIGHT";
	add_lua_global(&s, static_cast<unsigned int>(COMPONENT_ACTION_RIGHT));
	s = "COMPONENT_NAVIGATE_TO_NEXT";
	add_lua_global(&s, static_cast<unsigned int>(COMPONENT_NAVIGATE_TO_NEXT));
	s = "COMPONENT_NAVIGATE_TO_PREVIOUS";
	add_lua_global(&s, static_cast<unsigned int>(COMPONENT_NAVIGATE_TO_PREVIOUS));

	//publish mouse event as globals
	s = "MOUSE_BUTTON_LEFT_DOWN";
	add_lua_global(&s, static_cast<unsigned int>(MOUSE_BUTTON_LEFT_DOWN));
	s = "MOUSE_BUTTON_RIGHT_DOWN";
	add_lua_global(&s, static_cast<unsigned int>(MOUSE_BUTTON_RIGHT_DOWN));
	s = "MOUSE_BUTTON_MIDDLE_DOWN";
	add_lua_global(&s, static_cast<unsigned int>(MOUSE_BUTTON_MIDDLE_DOWN));
	s = "MOUSE_BUTTON_LEFT_UP";
	add_lua_global(&s, static_cast<unsigned int>(MOUSE_BUTTON_LEFT_UP));
	s = "MOUSE_BUTTON_RIGHT_UP";
	add_lua_global(&s, static_cast<unsigned int>(MOUSE_BUTTON_RIGHT_UP));
	s = "MOUSE_BUTTON_MIDDLE_UP";
	add_lua_global(&s, static_cast<unsigned int>(MOUSE_BUTTON_MIDDLE_UP));
	s = "UIMANAGER_MOUSE_MOVED";
	add_lua_global(&s, static_cast<unsigned int>(UIMANAGER_MOUSE_MOVED));
	//possibly publish ps3 controller events


	s = "UPDATE";
	add_lua_global(&s, static_cast<unsigned int>(UPDATE));

	//publish as triggerable events
	s = "RELOAD_UI";
	add_lua_event_code(&s, static_cast<unsigned int>(RELOAD_UI));
	s = "COMPONENT_REQUEST_FOCUS";
	add_lua_event_code(&s, static_cast<unsigned int>(COMPONENT_REQUEST_FOCUS));
	s = "COMPONENT_REQUEST_FOCUS_PREVIOUS";
	add_lua_event_code(&s, static_cast<unsigned int>(COMPONENT_REQUEST_FOCUS_PREVIOUS));

	//publish other data
	s = "UIMANAGER_FROM_LUA_BIAS";
	add_lua_global(&s, UIMANAGER_FROM_LUA_BIAS);

	//register keyboard events
	for(unsigned long e = KEYBOARD_KEY_CANCEL_DOWN; e <= KEYBOARD_KEY_OEMCLEAR_UP; ++e){
		register_event(e);
	}
	//register mouse events
	for(unsigned long e = MOUSE_POSITION; e <= MOUSE_BUTTON_X2_UP; ++e){
		register_event(e);
	}
	register_event(COMPONENT_ACTION_PERFORMED);
	register_event(COMPONENT_ACTION_LEFT);
	register_event(COMPONENT_ACTION_RIGHT);
	register_event(COMPONENT_NAVIGATE_TO_NEXT);
	register_event(COMPONENT_NAVIGATE_TO_PREVIOUS);
	register_event(COMPONENT_REQUEST_FOCUS);
	register_event(COMPONENT_REQUEST_FOCUS_PREVIOUS);
}

void UIManager::on_event( const unsigned long event_code , const Message& parameters ){
	static double lastX = -1.0;
	static double lastY = -1.0;
	static int last_X = 0;
	static int last_Y = 0;
	static map<ID, UIComponent::COMPONENT_TYPE> lastComponents;
	static map<ID, UIComponent::COMPONENT_TYPE> temp;

	unsigned long code = event_code;

	bool sendEventToAllComponents = false;
	
	if(event_code == MOUSE_POSITION){
		int _x = (int)parameters.get_integral_data(2);
		int _y = (int)parameters.get_integral_data(3);

		#ifdef _WIN32
			POINT ptCursor;
			ptCursor.x = _x;
			ptCursor.y = _y;
			GUITHREADINFO ti;
			ti.cbSize = sizeof(GUITHREADINFO);
			GetGUIThreadInfo(0, &ti);
			ScreenToClient( ti.hwndActive, &ptCursor );
			_x = ptCursor.x;
			_y = ptCursor.y;
		#endif		
		Vector2f screenPos = unproject(_x, _y);

		double x = screenPos.x;
		double y = screenPos.y;

		if(_x != last_X || _y != last_Y){
			for(map<ID, UIImage*>::iterator it = m_images.begin(); it != m_images.end(); ++it){
				Message mouseParameters(2, 2, 0);
				mouseParameters.add_integral_data(0, parameters.get_integral_data(0));
				mouseParameters.add_integral_data(1, parameters.get_integral_data(1));
				mouseParameters.add_real_data(0, x);
				mouseParameters.add_real_data(1, y);
				it->second->on_event(m_state, UIMANAGER_MOUSE_MOVED, mouseParameters);
			}
		}
		last_X = _x;
		last_Y = _y;
		lastX = x;
		lastY = y;
	}else if(event_code == COMPONENT_REQUEST_FOCUS){ 
		double intPart;
		double param = parameters.get_real_data(0);
		modf(param, &intPart);
		ID componentId = static_cast<ID>(intPart);
		if(componentId > MIN_ID){
			map<ID, unsigned int>::const_iterator it = m_navigation2.find(componentId);
			if(it != m_navigation2.end()){
				UIComponent* comp = get_component(componentId);
				if(comp && comp->is_visible() && it->second != m_currentTabIndex){
					m_lastTabIndex = m_currentTabIndex;
					m_currentTabIndex = it->second;
					//transfer_focus(it->first);
					transfer_focuss(comp);
				}
			}
		}
	}else if(event_code == COMPONENT_REQUEST_FOCUS_PREVIOUS){
		map<unsigned int, ID>::const_iterator it = m_navigation.find(m_lastTabIndex);
		if(it != m_navigation.end()){
			UIComponent* comp = get_component(it->second);
			if(comp && comp->is_visible()){
				unsigned int tmp = m_currentTabIndex;
				m_currentTabIndex = m_lastTabIndex;
				m_lastTabIndex = tmp;
				transfer_focuss(comp);
			}
		}
	}else if(event_code == COMPONENT_ACTION_PERFORMED || event_code == COMPONENT_ACTION_LEFT || event_code == COMPONENT_ACTION_RIGHT){
		if(m_focusedComponent && m_focusedComponentPtr && m_focusedComponentPtr->is_visible()){
			m_focusedComponentPtr->system_behaviour(event_code, parameters);
			m_focusedComponentPtr->on_event(m_state, event_code, parameters);
		}
	}else if(event_code == COMPONENT_NAVIGATE_TO_NEXT){
		map<unsigned int, ID>::const_iterator it;
		if(m_currentTabIndex == 0){
			if(m_navigation.size() > 0){
				it = m_navigation.begin();
				UIComponent* c = get_component(it->second);
				if(c && c->is_enabled() && m_currentTabIndex != it->first && c->is_visible()){
					m_lastTabIndex = m_currentTabIndex;
					m_currentTabIndex = it->first;
					transfer_focuss(c);
				}
			}
		}else{
			it = m_navigation.find(m_currentTabIndex);
			bool foundFocusableComponent = false;
			++it;
			while(!foundFocusableComponent && it != m_navigation.end()){
				foundFocusableComponent = navigation_check(it->second, it->first);
				++it;
			}
		}
	}else if(event_code == COMPONENT_NAVIGATE_TO_PREVIOUS){
		map<unsigned int, ID>::const_iterator it;
		if(m_currentTabIndex == 0){
			if(m_navigation.size() > 0){
				it = m_navigation.begin();
				UIComponent* c = get_component(it->second);
				if(c && c->is_enabled() && m_currentTabIndex != it->first && c->is_visible()){
					m_lastTabIndex = m_currentTabIndex;
					m_currentTabIndex = it->first;
					transfer_focuss(c);
				}
			}
		}else{

			it = m_navigation.find(m_currentTabIndex);
			unsigned int size = m_navigation.size();
			unsigned int i = 0;
			bool foundFocusableComponent = false;
			--it;
			while(!foundFocusableComponent && i < size && it != m_navigation.end()){
				
				foundFocusableComponent = navigation_check(it->second, it->first);
				--it;
				++i;
			}
			
		}
	}else if(event_code == MOUSE_BUTTON_LEFT_DOWN || event_code == MOUSE_BUTTON_LEFT_UP || event_code == MOUSE_POSITION_CHANGED){
		vector<UIComponent*> comps = find_components(lastX, lastY, true);

		temp.clear();
		//components that should receive the COMPONENT_ON_MOUSE_HOVER event are those that are not in "lastComponents" but are now in "comps"
		//components that should receive the COMPONENT_ON_MOUSE_OUT event are those that are in "lastComponents" but are no longer in "comps"
		//then lastComponents should be cleared and contain all components from comps
		for(vector<UIComponent*>::const_iterator it = comps.begin(); it != comps.end(); ++it){
			UIComponent* comp = *it;
			Message params(3, 2, 0);
			params.add_integral_data(0, parameters.get_integral_data(0));
			params.add_integral_data(1, parameters.get_integral_data(1));
			params.add_integral_data(2, comp->get_id());
			params.add_real_data(0, lastX);
			params.add_real_data(1, lastY);
			comp->system_behaviour(code, params);
			comp->on_event(m_state, code, parameters);
			/*
			if(event_code == MOUSE_BUTTON_LEFT_DOWN){
				comp->system_behaviour(COMPONENT_ACTION_PERFORMED, parameters);
				comp->on_event(m_state, COMPONENT_ACTION_PERFORMED, parameters);
			}
			*/
			temp.insert(pair<ID, UIComponent::COMPONENT_TYPE>(comp->get_id(), comp->get_type()));
			if(lastComponents.find(comp->get_id()) == lastComponents.end()){
				comp->on_event(m_state, COMPONENT_ON_MOUSE_HOVER, parameters);
			}
		}
		for(map<ID, UIComponent::COMPONENT_TYPE>::const_iterator it = lastComponents.begin(); it!= lastComponents.end(); ++it){
			if(temp.find(it->first) == temp.end()){
				UIComponent* c = NULL;
				if((c = get_component(it->first, it->second)) && c->is_visible()){
					c->on_event(m_state, COMPONENT_ON_MOUSE_OUT, parameters);
				}
			}
		}
		lastComponents.clear();
		for(map<ID, UIComponent::COMPONENT_TYPE>::const_iterator it = temp.begin(); it!= temp.end(); ++it){
			lastComponents.insert(pair<ID, UIComponent::COMPONENT_TYPE>(it->first, it->second));
		}

		if(event_code == MOUSE_POSITION_CHANGED){
			UIComponent* newFocus = find_component(lastX, lastY);//TODO potentially wasted effort, use the vector "comps"
			if(newFocus){
				map<ID, unsigned int>::const_iterator navIt;
				if((navIt = m_navigation2.find(newFocus->get_id())) != m_navigation2.end() && m_currentTabIndex != navIt->second){
					m_lastTabIndex = m_currentTabIndex;
					m_currentTabIndex = navIt->second;
					transfer_focuss(newFocus);
				}
			}			
		}
		if(event_code == MOUSE_BUTTON_LEFT_DOWN && m_focusedComponentPtr && m_focusedComponentPtr->is_visible()){
			Vector2f screenPos(lastX, lastY);
			float cx = m_focusedComponentPtr->get_x();
			float cy = m_focusedComponentPtr->get_y();
			BoundingBox2 d(Vector2f(cx, cy), Vector2f(cx + m_focusedComponentPtr->get_width(), cy + m_focusedComponentPtr->get_height()));
			if(d.contains(screenPos)){
				m_focusedComponentPtr->system_behaviour(COMPONENT_ACTION_PERFORMED, parameters);
				m_focusedComponentPtr->on_event(m_state, COMPONENT_ACTION_PERFORMED, parameters);
			}
		}
	}else if(event_code >= KEYBOARD_KEY_CANCEL_DOWN && event_code <= KEYBOARD_KEY_DOUBLEQUOTES_UP/*KEYBOARD_KEY_OEMCLEAR_DOWN*/){
		if(m_focusedComponent > 0xff && m_focusedComponentPtr && m_focusedComponentPtr->is_visible()){
			m_focusedComponentPtr->system_behaviour(code);
			sendEventToAllComponents = !m_focusedComponentPtr->is_textbox();
			/*
			UIComponent* c = get_component(m_focusedComponent);
			if(c){
				c->system_behaviour(code);
				sendEventToAllComponents = !c->is_textbox();
			}else{
				sendEventToAllComponents = true;
			}
			*/
		}
	}else{
		if ( event_code == m_update_rate ){
			//this reload_ui can be triggered from lua, this mechanism is to make sure that we're not within a lua call when reload_ui is called, because it resets the lua state
			if (m_reload_pending){
				reload_ui();
				m_reload_pending = false;
			}

			code = UPDATE;
			sendEventToAllComponents = true;
		}
		else if ( event_code == INITIALISE )
			initialise();
		else if ( event_code == EXIT )
			terminate();
		else if ( event_code == UIMANAGER_CHANGE_UPDATE_RATE )
			change_update_rate(static_cast<unsigned long>(parameters.get_integral_data(2)));
		else if ( event_code == RELOAD_UI )
			m_reload_pending = true;
		else if ( event_code == LOAD_SCRIPT )
			add_script(static_cast<string*>(parameters.get_pointer(0)));
		else if ( event_code == RUN_SCRIPT )
			run_script();
		else if ( event_code == LOAD_AND_RUN_SCRIPT ){
			if ( add_script(static_cast<string*>(parameters.get_pointer(0))) )
				run_script();
		}
		else if ( event_code == ADD_NULL_VARIABLE )
			add_lua_global(static_cast<string*>(parameters.get_pointer(0)));
		else if ( event_code == ADD_BOOL_VARIABLE )
			add_lua_global(static_cast<string*>(parameters.get_pointer(0)),( parameters.get_integral_data(2)  ?  1 : 0 ));
		else if ( event_code == ADD_INT_VARIABLE )
			add_lua_global(static_cast<string*>(parameters.get_pointer(0)),static_cast<int>(parameters.get_integral_data(2)));
		else if ( event_code == ADD_UINT_VARIABLE )
			add_lua_global(static_cast<string*>(parameters.get_pointer(0)),static_cast<unsigned int>(parameters.get_integral_data(2)));
		else if ( event_code == ADD_DOUBLE_VARIABLE )
			add_lua_global(static_cast<string*>(parameters.get_pointer(0)),static_cast<double>(parameters.get_real_data(0)));
		else if ( event_code == ADD_STRING_VARIABLE )
			add_lua_global(static_cast<string*>(parameters.get_pointer(0)),static_cast<string*>(parameters.get_pointer(1)));
		else if ( event_code == ADD_EVENT_VARIABLE )
			add_lua_event_code(static_cast<string*>(parameters.get_pointer(0)),static_cast<unsigned int>(parameters.get_integral_data(2)));
		else if ( event_code == ADD_FUNCTION_VARIABLE )
			add_lua_function(static_cast<string*>(parameters.get_pointer(0)),static_cast<lua_CFunction>(parameters.get_pointer(1)));
		else if ( event_code == REMOVE_NULL_VARIABLE )
			remove_lua_global(static_cast<string*>(parameters.get_pointer(0)));
		else if ( event_code == REMOVE_BOOL_VARIABLE )
			remove_lua_bool_global(static_cast<string*>(parameters.get_pointer(0)));
		else if ( event_code == REMOVE_INT_VARIABLE )
			remove_lua_int_global(static_cast<string*>(parameters.get_pointer(0)));
		else if ( event_code == REMOVE_UINT_VARIABLE )
			remove_lua_uint_global(static_cast<string*>(parameters.get_pointer(0)));
		else if ( event_code == REMOVE_DOUBLE_VARIABLE )
			remove_lua_double_global(static_cast<string*>(parameters.get_pointer(0)));
		else if ( event_code == REMOVE_STRING_VARIABLE )
			remove_lua_string_global(static_cast<string*>(parameters.get_pointer(0)));
		else if ( event_code == REMOVE_EVENT_VARIABLE )
			remove_lua_event_code(static_cast<string*>(parameters.get_pointer(0)));
		else if ( event_code == REMOVE_FUNCTION_VARIABLE )
			remove_lua_function(static_cast<string*>(parameters.get_pointer(0)));
		else
			sendEventToAllComponents = is_event_available(event_code);
	}

	if(sendEventToAllComponents){
		for(map<ID, Button*>::const_iterator it = m_buttons.begin(); it!= m_buttons.end(); ++it){
			(*it).second->on_event(m_state,code, parameters);
		}
		for(map<ID, Checkbox*>::const_iterator it = m_checkboxes.begin(); it!= m_checkboxes.end(); ++it){
			(*it).second->on_event(m_state,code, parameters);
		}
		for(map<ID, UIImage*>::const_iterator it = m_images.begin(); it!= m_images.end(); ++it){
			(*it).second->on_event(m_state,code, parameters);
		}
		for(map<ID, Label*>::const_iterator it = m_labels.begin(); it!= m_labels.end(); ++it){
			(*it).second->on_event(m_state,code, parameters);
		}
		for(map<ID, Slider*>::const_iterator it = m_sliders.begin(); it!= m_sliders.end(); ++it){
			(*it).second->on_event(m_state,code, parameters);
		}
		for(map<ID, Textbox*>::const_iterator it = m_textboxes.begin(); it!= m_textboxes.end(); ++it){
			(*it).second->on_event(m_state,code, parameters);
		}
		
		for(map<ID, Container*>::const_iterator it = m_containers.begin(); it!= m_containers.end(); ++it){
			(*it).second->on_event(m_state,code, parameters);
		}
	}
}

bool UIManager::is_event_available( const unsigned int code ) const{
	bool return_value = false;
	map<string,unsigned int>::const_iterator it = m_lua_event_globals.begin();

	while ( !return_value  &&  it != m_lua_event_globals.end() ){
		if ( it->second == code )
			return_value = true;
		else
			++it;
	}

	return return_value;
}

UIManager::~UIManager(){
	light_terminate();
}

void UIManager::setup_renderer(IRenderer *renderer)
{
	m_renderer = renderer;
}

	}//namespace core
}//namespace TEXEngine
