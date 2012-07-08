#include "uiImage.hpp"
#include "../math/vector.hpp"
#include "uiManager.hpp"

using namespace TEXEngine::Math;
using namespace TEXEngine::Core;
using namespace TEXEngine::Util;

namespace TEXEngine{
	namespace UI{

UIImage::UIImage(const ID id, const unsigned int texid) : UIComponent(id), m_tex_u_start_shift(0.0f), m_tex_u_end_shift(0.0f), m_tex_v_start_shift(0.0f), m_tex_v_end_shift(0.0f), m_texid(texid), 
	m_u1(0.0f), m_u2(1.0f), m_u3(1.0f), m_u4(0.0f), m_v1(1.0f), m_v2(1.0f), m_v3(0.0f), m_v4(0.0f){

}

UIImage::UIImage(const ID id, const unsigned int texid, const float u1, const float v1, const float u2, const float v2, const float u3, const float v3, const float u4, const float v4) : 
	UIComponent(id), m_tex_u_start_shift(0.0f), m_tex_u_end_shift(0.0f), m_tex_v_start_shift(0.0f), m_tex_v_end_shift(0.0f), m_texid(texid), 
	m_u1(u1), m_u2(u2), m_u3(u3), m_u4(u4), m_v1(v1), m_v2(v2), m_v3(v3), m_v4(v4){

}



void UIImage::render() const{
	/*
	Vector2f uv1(0.0, 1.0);
	Vector2f uv2(1.0, 1.0);
	Vector2f uv3(1.0, 0.0);
	Vector2f uv4(0.0, 0.0);
	*/
	Vector2f uv1(m_u1+m_tex_u_start_shift, m_v1+m_tex_v_end_shift);
	Vector2f uv2(m_u2+m_tex_u_end_shift, m_v2+m_tex_v_end_shift);
	Vector2f uv3(m_u3+m_tex_u_end_shift, m_v3+m_tex_v_start_shift);
	Vector2f uv4(m_u4+m_tex_u_start_shift, m_v4+m_tex_v_start_shift);
	//call renderbox with get_id and these tex coords
	UIManager::get().render_box(get_x(), get_y(), get_width(), get_height(), get_background_color(), m_texid, uv1, uv2, uv3, uv4);
}

UIImage::~UIImage(){
}

	}//namespace UI
}//namespace TEXEngine
