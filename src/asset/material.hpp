#ifndef TEXENGINE_MATERIAL_HPP_INCLUDED
#define TEXENGINE_MATERIAL_HPP_INCLUDED

#include "../util/color.hpp"

using TEXEngine::Util::ColorRGBAf;

namespace TEXEngine {
	
	namespace Core {
		class AssetManager; /* Forward declaration */
	}
	
	using Core::AssetManager;

	namespace Asset {

class Material
{
	friend class AssetManager;
	protected:
		Material();
		Material(const ColorRGBAf &ambient, 
			const ColorRGBAf &diffuse, 
			const ColorRGBAf &specular,
			const ColorRGBAf &emission,
			float shininess);

	public:
		// Accessors
		const ColorRGBAf &ambient() const;
		const ColorRGBAf &diffuse() const;
		const ColorRGBAf &specular() const;
		const ColorRGBAf &emission() const;
		const float shininess() const;

		// Mutators
		void ambient(const ColorRGBAf &ambient);
		void diffuse(const ColorRGBAf &diffuse);
		void specular(const ColorRGBAf &specular);
		void emission(const ColorRGBAf &emission);
		void shininess(float shininess);

	private:
		ColorRGBAf m_ambient;
		ColorRGBAf m_diffuse;
		ColorRGBAf m_specular;
		ColorRGBAf m_emission;
		float m_shininess;
};

	} /* namespace Asset */
} /* Namespace TEXEngine */

#endif /* TEXENGINE_MATERIAL_HPP_INCLUDED */