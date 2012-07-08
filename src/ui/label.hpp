#ifndef TEXENGINE_LABEL_HPP_INCLUDED
#define TEXENGINE_LABEL_HPP_INCLUDED

#include "textComponent.hpp"

namespace TEXEngine{
	namespace Core{
		class UIManager;
	}//forward declaration

	namespace UI{

class Label : public TEXEngine::UI::TextComponent{
friend class TEXEngine::Core::UIManager;

protected:
	void render() const;

	void system_behaviour(const unsigned long, const TEXEngine::Core::Message&);

	Label(const ID id, const ID font, const char*);

	COMPONENT_TYPE get_type() const{
		return COMPONENT_TYPE_LABEL;
	}


public:

	~Label();

};

	}//namespace UI
}//namespace TEXEngine

#endif /* TEXENGINE_LABEL_HPP_INCLUDED */