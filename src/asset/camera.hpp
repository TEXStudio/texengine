#ifndef TEXENGINE_CAMERA_HPP_INCLUDED
#define TEXENGINE_CAMERA_HPP_INCLUDED

#include "../math/vector.hpp"
#include "../math/matrix.hpp"
#include "assetEnum.h"

using TEXEngine::Math::Vector3f;
using TEXEngine::Math::Matrix4x4f;
using TEXEngine::Math::Vector4f;
using TEXEngine::Asset::TEXENG_CAMERA_TYPE;
using TEXEngine::Asset::TEXENG_CAMERA_CHASE;
using TEXEngine::Asset::TEXENG_CAMERA_FPS;

namespace TEXEngine {
		
	namespace Core {
		class AssetManager; /* Forward declaration */
	}

	using Core::AssetManager;

	namespace Asset {

class Camera
{
	friend class AssetManager;
	protected:
		Camera();
		~Camera();

	public:
		const Vector3f &position() const;
		const Vector3f &target() const;
		const Vector3f &up() const;

		void position(const Vector3f &v);
		void target(const Vector3f &v);
		void up(const Vector3f &v);

		const Matrix4x4f &view_matrix();

		void render(); //Miust be called in the rendering loop right after begin scene

		void move_forward(const float amount);
		void strafe(const float amount);
		void set_yaw(const float angle);
		void set_pitch(const float angle);

		void set_lookat_offset(const Vector3f& offset);
		void set_forward(const Vector3f& vec);
		void set_target_velocity(const Vector3f& vec);
		void set_camera_type(const TEXEngine::Asset::TEXENG_CAMERA_TYPE cam_type);
		void set_chase_cam_position(const Vector3f& vec);

		const TEXENG_CAMERA_TYPE get_camera_type();

	protected:
		Vector3f m_position;
		Vector3f m_target;
		Vector3f m_up;
		Vector3f m_forward;
		Vector3f m_right;
		Vector3f m_rotation;
		bool m_view_dir_changed;
		Vector3f m_lookat_offset;
		Vector3f m_target_velocity;
		Vector3f m_chase_cam_position;

		void get_view_direction();

		void matrix_to_1D_array(float* outArray, const Matrix4x4f& matrix);
		void matrix_to_1D_array(float* outArray, const Matrix4x4f* matrix);
		void array_1D_to_matrix(Matrix4x4f& out_matrix, float* in_array);
		void array4x3_1D_to_matrix(Matrix4x4f& out_matrix, float* in_array);

		void update();

		void update_chase_camera();

		Matrix4x4f m_view_matrix;
		TEXENG_CAMERA_TYPE m_camera_type;

};

	} /* namespace Asset */
} /* Namespace TEXEngine */

#endif /* TEXENGINE_CAMERA_HPP_INCLUDED */