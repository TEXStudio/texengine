#include "material.hpp"

namespace TEXEngine {
	namespace Asset {

Material::Material()
	: m_shininess(1)
{}

Material::Material(const ColorRGBAf &ambient, 
			const ColorRGBAf &diffuse, 
			const ColorRGBAf &specular,
			const ColorRGBAf &emission,
			float shininess)
	: m_ambient(ambient), m_diffuse(diffuse), m_specular(specular),
	  m_emission(emission), m_shininess(shininess)
{}

const ColorRGBAf &Material::ambient() const
{
	return m_ambient;
}

const ColorRGBAf &Material::diffuse() const
{
	return m_diffuse;
}

const ColorRGBAf &Material::specular() const
{
	return m_specular;
}

const ColorRGBAf &Material::emission() const
{
	return m_emission;
}

const float Material::shininess() const
{
	return m_shininess;
}

void Material::ambient(const ColorRGBAf &ambient)
{
	m_ambient = ambient;
}

void Material::diffuse(const ColorRGBAf &diffuse)
{
	m_diffuse = diffuse;
}

void Material::specular(const ColorRGBAf &specular)
{
	m_specular = specular;
}

void Material::emission(const ColorRGBAf &emission)
{
	m_emission = emission;
}

void Material::shininess(float shininess)
{
	m_shininess = shininess;
}
		
	} /* namespace Asset */
} /* Namespace TEXEngine */