#include "light.hpp"

namespace TEXEngine {
	namespace Asset {

Light::Light()
	: m_cutoff(45.0f), m_exponent(2.0f), 
	  m_constn_attenuation(0.9f),
	  m_linear_attenuation(0.01f),
	  m_quadrc_attenuation(0.001f),
	  m_index(0), m_type(TEXENG_LIGHT_DIRECTIONAL), m_is_active(true)
{}

Light::Light(const Vector3f &position,
			const ColorRGBAf &ambient, 
			const ColorRGBAf &diffuse, 
			const ColorRGBAf &specular)
	: m_position(position),
	  m_ambient(ambient), m_diffuse(diffuse), m_specular(specular),
	  m_cutoff(45.0f), m_exponent(2.0f), 
	  m_constn_attenuation(0.9f),
	  m_linear_attenuation(0.01f),
	  m_quadrc_attenuation(0.001f),
	  m_index(0), m_type(TEXENG_LIGHT_DIRECTIONAL), m_is_active(true)

{}

const Vector3f &Light::position() const
{
	return m_position;
}

const ColorRGBAf &Light::ambient() const
{
	return m_ambient;
}

const ColorRGBAf &Light::diffuse() const
{
	return m_diffuse;
}

const ColorRGBAf &Light::specular() const
{
	return m_specular;
}

TEXENG_LIGHT_TYPE Light::type() const
{
	return m_type;
}

unsigned int Light::index() const
{
	return m_index;
}

const Vector3f &Light::target() const
{
	return m_target;
}

const Vector3f &Light::direction() const
{
	return m_direction;
}

float Light::cutoff() const
{
	return m_cutoff;
}

float Light::exponent() const
{
	return m_exponent;
}

float Light::constn_attenuation() const
{
	return m_constn_attenuation;
}

float Light::linear_attenuation() const
{
	return m_linear_attenuation;
}

float Light::quadrc_attenuation() const
{
	return m_quadrc_attenuation;
}

void Light::position(const Vector3f &position)
{
	m_position = position;
	m_direction = (m_target - m_position).normalized();
}

void Light::ambient(const ColorRGBAf &ambient)
{
	m_ambient = ambient;
}

void Light::diffuse(const ColorRGBAf &diffuse)
{
	m_diffuse = diffuse;
}

void Light::specular(const ColorRGBAf &specular)
{
	m_specular = specular;
}

void Light::type(TEXENG_LIGHT_TYPE type)
{
	m_type = type;
}

void Light::index(unsigned int idx)
{
	m_index = idx > 7 ? 7 : idx;
}

void Light::target(const Vector3f &tar)
{
	m_target = tar;
	m_direction = (m_target - m_position).normalized();
}

void Light::cutoff(float val)
{
	m_cutoff = val;
}

void Light::exponent(float val)
{
	m_exponent = val;
}

void Light::constn_attenuation(float val)
{
	m_constn_attenuation = val;
}

void Light::linear_attenuation(float val)
{
	m_linear_attenuation = val;
}

void Light::quadrc_attenuation(float val)
{
	m_quadrc_attenuation = val;
}

void Light::activate()
{
	m_is_active = true;
}

void Light::deactivate()
{
	m_is_active = false;
}

bool Light::is_active() const
{
	return m_is_active;
}

bool Light::toggle()
{
	if (m_is_active)
		deactivate();
	else
		activate();

	return m_is_active;
}

	} /* namespace TEXEngine */
} /* namespace Asset */