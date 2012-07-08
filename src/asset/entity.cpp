#include "entity.hpp"

namespace TEXEngine {
	namespace Asset {

Entity::Entity()
	: m_mesh(0),
	  m_material(0),
	  m_shader(0),
	  m_physics_obj(NULL)
{}

Entity::~Entity()
{}

unsigned int Entity::mesh() const
{
	return m_mesh;
}

unsigned int Entity::material() const
{
	return m_material;
}

unsigned int Entity::shader() const
{
	return m_shader;
}

PhysicsObj *Entity::physics_obj() const
{
	return m_physics_obj;
}

unsigned int Entity::texture(TEXENG_TEXTURE_TYPE type)
{
	std::map<TEXENG_TEXTURE_TYPE, unsigned int>::iterator it = m_textures.find(type);
	if(it != m_textures.end())
		return (*it).second;
	return 0;
}

void Entity::mesh(unsigned int id)
{
	m_mesh = id;
}

void Entity::material(unsigned int id)
{
	m_material = id;
}

void Entity::shader(unsigned int id)
{
	m_shader = id;
}

void Entity::physics_obj(PhysicsObj *obj)
{
	m_physics_obj = obj;
}
		
void Entity::texture(TEXENG_TEXTURE_TYPE type, unsigned int id)
{
	m_textures[type] = id;
}

Matrix4x4f &Entity::matrix()
{
	return m_matrix;
}

void Entity::matrix(const Matrix4x4f &mat)
{
	m_matrix = mat;
}

void Entity::collidable(bool status)
{
	m_is_collidable = status;
}

bool Entity::is_collidable() const
{
	return m_is_collidable;
}

void Entity::show()
{
	m_is_visible = true;
}

void Entity::hide()
{
	m_is_visible = false;
}

void Entity::visibility(bool status)
{
	m_is_visible = status;
}

void Entity::toggle_visibility()
{
	m_is_visible = !m_is_visible;
}

bool Entity::is_visible() const
{
	return m_is_visible;
}

void Entity::activate()
{
	m_is_active = true;
}

void Entity::deactivate()
{
	m_is_active = false;
}

void Entity::state(bool status)
{
	m_is_active = status;
}

void Entity::toggle_state()
{
	m_is_active = !m_is_active;
}

bool Entity::is_active() const
{
	return m_is_active;
}

	} /* namespace Asset */
} /* Namespace TEXEngine */