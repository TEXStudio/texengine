#ifndef TEXENGINE_CONTAINER_HPP_INCLUDED
#define TEXENGINE_CONTAINER_HPP_INCLUDED

#include <map>
#include "uiComponent.hpp"
#include "uiUtility.hpp"

namespace TEXEngine{
	namespace Core{
		class UIManager;
	}//forward declaration

	namespace UI{

class Container : public TEXEngine::UI::UIComponent{
	friend class TEXEngine::Core::UIManager;

private:
	std::multimap<ZIndex, ID> m_components;


protected:
	void render() const;

	Container(const ID, const float x = 0.0f, const float y = 0.0f, const float w = 0.0f, const float h = 0.0f);

	bool m_renderable;

	COMPONENT_TYPE get_type() const{
		return COMPONENT_TYPE_CONTAINER;
	}

public:

	~Container();

	//attach the given component to this container
	void attach(const ID, const ZIndex = 0);
	
	//removes the given component from this container
	void detach(const ID);

	void set_renderable(const bool r){
		m_renderable = r;
	}
	
};

	}//namespace UI
}//namespace TEXEngine

#endif /* TEXENGINE_CONTAINER_HPP_INCLUDED */