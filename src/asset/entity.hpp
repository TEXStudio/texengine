#ifndef TEXENGINE_ENTITY_HPP_INCLUDED
#define TEXENGINE_ENTITY_HPP_INCLUDED

#include <map>
#include "../math/matrix.hpp"
#include "assetEnum.h"

using TEXEngine::Math::Matrix4x4f;

namespace TEXEngine {
	
	namespace Core {
		class AssetManager; /* Forward declaration */
	}

	namespace Physics {
		class PhysicsObj;
	}

	using Core::AssetManager;
	using Physics::PhysicsObj;

	namespace Asset {

class Entity
{
	friend class AssetManager;
	protected:
		Entity();
		~Entity();

	public:
		unsigned int mesh() const;
		unsigned int material() const;
		unsigned int shader() const;
		PhysicsObj *physics_obj() const; 
		unsigned int texture(TEXENG_TEXTURE_TYPE type);

		void mesh(unsigned int id);
		void material(unsigned int id);
		void shader(unsigned int id);
		void physics_obj(PhysicsObj *obj);
		void texture(TEXENG_TEXTURE_TYPE type, unsigned int id);

		Matrix4x4f &matrix();
		void matrix(const Matrix4x4f &mat);
	
		void collidable(bool status);
		bool is_collidable() const;

		void show();
		void hide();
		void visibility(bool status);
		void toggle_visibility();
		bool is_visible() const;

		void activate();
		void deactivate();
		void state(bool status);
		void toggle_state();
		bool is_active() const;

	private:
		unsigned int m_mesh;
		unsigned int m_material;
		unsigned int m_shader;
		PhysicsObj *m_physics_obj;
		std::map<TEXENG_TEXTURE_TYPE, unsigned int> m_textures;

		Matrix4x4f m_matrix;

		bool m_is_collidable;
		bool m_is_visible;
		bool m_is_active;
};

	} /* namespace Asset */
} /* Namespace TEXEngine */

#endif /* TEXENGINE_ENTITY_HPP_INCLUDED */