#ifndef TEXENGINE_UIIMAGE_HPP_INCLUDED
#define TEXENGINE_UIIMAGE_HPP_INCLUDED

#include "uiComponent.hpp"

namespace TEXEngine{
	namespace Core{
		class UIManager;
	}//forward declaration
	namespace UI{

class UIImage : public TEXEngine::UI::UIComponent{
	friend class TEXEngine::Core::UIManager;

protected:
	void render() const;

	//UIImage(const ID);

	UIImage(const ID, const unsigned texid);
	UIImage(const ID, const unsigned texid, const float u1, const float v1, const float u2, const float v2, const float u3, const float v3, const float u4, const float v4);

	COMPONENT_TYPE get_type() const{
		return COMPONENT_TYPE_IMAGE;
	}

private:

	float m_tex_u_start_shift; //this is added the u1 and u4
	float m_tex_v_start_shift; //this is added to v3 and v4
	float m_tex_u_end_shift; //this is added to u2 and u3
	float m_tex_v_end_shift; //this is added to v1 and v2

	float m_u1, m_u2, m_u3, m_u4, m_v1, m_v2, m_v3, m_v4;

	unsigned int m_texid;

public:

	~UIImage();

	void set_texu_shift(const float ustart, const float uend){
		m_tex_u_start_shift = ustart;
		m_tex_u_end_shift = uend;
	}

	void set_texv_shift(const float vstart, const float vend){
		m_tex_v_start_shift = vstart;
		m_tex_v_end_shift = vend;
	}

	float get_texu_shift_start() const{
		return m_tex_u_start_shift;
	}

	float get_texu_shift_end() const{
		return m_tex_u_end_shift;
	}

	float get_texv_shift_start() const{
		return m_tex_v_start_shift;
	}

	float get_texv_shift_end() const{
		return m_tex_v_end_shift;
	}

};

	}//namespace UI
}//namespace TEXEngine

#endif /* TEXENGINE_IMAGE_HPP_INCLUDED */