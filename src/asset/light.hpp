#ifndef TEXENGINE_LIGHT_HPP_INCLUDED
#define TEXENGINE_LIGHT_HPP_INCLUDED

#include "../math/vector.hpp"
#include "../util/color.hpp"
#include "assetEnum.h"

using TEXEngine::Math::Vector3f;
using TEXEngine::Util::ColorRGBAf;
using TEXEngine::Asset::TEXENG_LIGHT_TYPE;
using TEXEngine::Asset::TEXENG_LIGHT_POINT;
using TEXEngine::Asset::TEXENG_LIGHT_SPOT;
using TEXEngine::Asset::TEXENG_LIGHT_DIRECTIONAL;

namespace TEXEngine {
	namespace Core {
		class AssetManager; /* Forward declaration */
	}
	
	using Core::AssetManager;

	namespace Asset {

class Light
{
	friend class AssetManager;
	protected:
		Light();
		Light(const Vector3f &position,
			const ColorRGBAf &ambient, 
			const ColorRGBAf &diffuse, 
			const ColorRGBAf &specular);

	public:
		const Vector3f &position() const;
		const ColorRGBAf &ambient() const;
		const ColorRGBAf &diffuse() const;
		const ColorRGBAf &specular() const;

		TEXENG_LIGHT_TYPE type() const;

		unsigned int index() const;

		const Vector3f &target() const;
		const Vector3f &direction() const;

		float cutoff() const;
		float exponent() const;

		float constn_attenuation() const;
		float linear_attenuation() const;
		float quadrc_attenuation() const;

		void position(const Vector3f &position);
		void ambient(const ColorRGBAf &ambient);
		void diffuse(const ColorRGBAf &diffuse);
		void specular(const ColorRGBAf &specular);

		void type(TEXENG_LIGHT_TYPE type);

		void index(unsigned int idx);

		void target(const Vector3f &tar);
		
		void cutoff(float val);
		void exponent(float val);

		void constn_attenuation(float val);
		void linear_attenuation(float val);
		void quadrc_attenuation(float val);

		void activate();
		void deactivate();
		bool toggle();
		bool is_active() const;

	private:
		Vector3f m_position;
		ColorRGBAf m_ambient;
		ColorRGBAf m_diffuse;
		ColorRGBAf m_specular;

		Vector3f m_target;
		Vector3f m_direction;

		float m_cutoff;
		float m_exponent;

		float m_constn_attenuation;
		float m_linear_attenuation;
		float m_quadrc_attenuation;

		unsigned int m_index;
		TEXENG_LIGHT_TYPE m_type;
		bool m_is_active;
};

	}/* namespace Asset */
} /* Namespace TEXEngine */

#endif /* TEXENGINE_LIGHT_HPP_INCLUDED */