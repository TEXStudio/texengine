#include "container.hpp"
#include "uiManager.hpp"

using namespace TEXEngine::Core;
using namespace TEXEngine::Util;
using namespace std;

namespace TEXEngine{
	namespace UI{

Container::Container(const ID id, const float x, const float y, const float w, const float h) : UIComponent(id), m_renderable(false){
	set_x(x);
	set_y(y);
	set_width(w);
	set_height(h);
}

void Container::render() const{
	if(is_visible()){
		if(m_renderable){
			float x = get_x();
			float y = get_y();
			float w = get_width();
			float h = get_height();
			UIManager::get().render_box(x, y, w, h, get_background_color());

			//Draw outline if needed
			if(m_render_border){
				render_border();
			}
		}
		
		for(multimap<ZIndex, ID>::const_iterator it = m_components.begin(); it != m_components.end(); ++it){
			UIComponent* comp = UIManager::get().get_component(it->second);
			if(comp && comp->is_visible()){
				comp->render();
			}
		}
	}
}

void Container::attach(const ID id, const ZIndex z){
	if(id == get_id())
		return;// this prevents some nasty overflows because if you're attaching to yourself then when we're calling get_x, get_y, or any of the recursive functions then we'll be hitting a stack overflow at some point

	UIComponent* comp = UIManager::get().get_component(id);
	if(comp){
		//if the comp that wants to attach to me is a parent, do nothing
		bool doNothing = false;
		if(comp->get_type() == COMPONENT_TYPE_CONTAINER){
			//ID pid = comp->get_parent();
			ID pid = get_parent();
			while(!doNothing && pid != 0){
				if(id == pid) {
					doNothing = true;
					break;
				}
				Container* next = UIManager::get().get_container(pid);
				if(next){
					pid = next->get_parent();
				}else break;
			}
		}
		if(!doNothing){

			//if the comp that wants to attach to me already has a parent, detach the comp from that parent
			if(comp->get_parent() != 0){
				Container* cont = UIManager::get().get_container(comp->get_parent());
				if(cont){
					cont->detach(id);
				}
			}

			comp->set_parent(get_id());
			//if this is called with an ID that's already in the list, then it will be added again
			m_components.insert(pair<ZIndex, ID>(z, id));
		}
	}
}

void Container::detach(const ID id){
	multimap<ZIndex, ID>::iterator it = m_components.begin();

	while( it != m_components.end() ) {
		if ( it->second == id ){
			m_components.erase(it++);	//since it's possible we have a component attached several times, we can't break out of here
		}else
			++it;
	}
}

Container::~Container(){
	for(multimap<ZIndex, ID>::const_iterator it = m_components.begin(); it != m_components.end(); ++it){
		UIComponent* comp = UIManager::get().get_component(it->second);
		if(comp)
			comp->set_parent(0);
	}
}

	}//namespace UI
}//namespace TEXEngine