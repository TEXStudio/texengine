#include "camera.hpp"
#include "../renderer/renderManager.hpp"
#include <btBulletDynamicsCommon.h>

//For openGL calls..they are needed 
#ifdef _WIN32
#include <GL/GLU.h>
#include <GL/GL.h>
#include "../port/pcRenderer.hpp"
#define GLFUNC_LOOKAT gluLookAt
#endif

#include "../log/logmanager.hpp"
using TEXEngine::Core::LogManager;
using TEXEngine::Math::scalar_t;
using TEXEngine::Math::cross;
using TEXEngine::Core::RenderManager;

namespace TEXEngine {
	namespace Asset {

Camera::Camera()
	: m_position(Vector3f(0.0f, 0.0f, 0.0f)),
	  m_target(Vector3f(0.0f, 0.0f, 1.0f)),
	  m_up(Vector3f(0.0f, 1.0f, 0.0f)),
	  m_forward(Vector3f(0.0f, 0.0f, -1.0f)),
	  m_right(Vector3f(1.0f, 0.0f, 0.0f)),
	  m_rotation(Vector3f(0.0f, 0.0f, 0.0f)),
	  m_view_dir_changed(false),
	  m_lookat_offset(Vector3f(0.0f, 0.0f, 0.0f)),
	  m_target_velocity(Vector3f(0.0f, 0.0f, 0.0f)),
	  m_chase_cam_position(Vector3f(0.0f, 0.0f, 0.0f))
{
	m_camera_type = TEXENG_CAMERA_CHASE;
}

Camera::~Camera()
{}

const Vector3f &Camera::position() const
{
	return m_position;
}

const Vector3f &Camera::target() const
{
	return m_target;
}

const Vector3f &Camera::up() const
{
	return m_up;
}

void Camera::position(const Vector3f &v)
{
	m_position = v;
	//if(m_camera_type == TEXENG_CAMERA_STATIC)
	{
		update();
	}
}

void Camera::target(const Vector3f &v)
{
	m_target = v;
	//if(m_camera_type == TEXENG_CAMERA_STATIC)
	{
		update();
	}
}

void Camera::up(const Vector3f &v)
{
	m_up = v;
	//if(m_camera_type == TEXENG_CAMERA_STATIC)
	{
		update();
	}
}

const Matrix4x4f &Camera::view_matrix()
{
	return m_view_matrix;
}

void Camera::update()
{
	// Calculate the new axis;
	Vector3f rz = (m_target - m_position).normalized(); 
	Vector3f rx = cross(rz, m_up).normalized();
	Vector3f ry = cross(rx, rz).normalized();

	m_view_matrix.load_identity();

	m_view_matrix *= Matrix4x4f(
		rx.x, rx.y, rx.z, 0.0f,
		ry.x, ry.y, ry.z, 0.0f,
		rz.x, rz.y, rz.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	m_view_matrix.translate(m_position);
	m_view_matrix.transpose();
}

void Camera::set_forward(const Vector3f& vec)
{
	m_forward = vec;
}

void Camera::set_target_velocity(const Vector3f& vec)
{
	m_target_velocity = vec;
}

void Camera::set_chase_cam_position(const Vector3f& vec)
{
	m_chase_cam_position = vec;
}


void Camera::render()
{
	//update the model view matri
	glMatrixMode(GL_MODELVIEW);

	if(m_camera_type == TEXENG_CAMERA_CHASE)
	{
		update_chase_camera();
	}

	if(m_camera_type == TEXENG_CAMERA_FPS)
	{
		float fXRotation = (float)m_rotation.x;
		float fYRotation = (float)m_rotation.y;
		float fZRotation = (float)m_rotation.z;

		float fXTranslation = (float)m_position.x;
		float fYTranslation = (float)m_position.y;
		float fZTranslation = (float)m_position.z;

		glRotatef(-fXRotation, 1.0f, 0.0f, 0.0f);
		glRotatef(-fYRotation, 0.0f, 1.0f, 0.0f);
		glRotatef(-fZRotation, 0.0f, 0.0f, 1.0f);
		glTranslatef(-fXTranslation, -fYTranslation, -fZTranslation);
	}
}

void Camera::matrix_to_1D_array(float* outArray, const Matrix4x4f* matrix)
{
	Matrix4x4f mat = *matrix;
	for(int i = 0; i < 16; i++)
	{
		outArray[i] = (float)*mat[i];
	}
}
void Camera::matrix_to_1D_array(float* outArray, const Matrix4x4f& matrix)
{
	Matrix4x4f mat = matrix;
	for(int i = 0; i < 16; i++)
	{
		outArray[i] = (float)*mat[i];
	}
}

void Camera::array_1D_to_matrix(Matrix4x4f& out_matrix, float* in_array)
{
	for(int i = 0; i < 16; i++)
	{
		*out_matrix[i] = (scalar_t)in_array[i];
	}
}

void Camera::array4x3_1D_to_matrix(Matrix4x4f& out_matrix, float* in_array)
{
	out_matrix.load_identity();

	*out_matrix[0] = (scalar_t) in_array[0];
	*out_matrix[1] = (scalar_t) in_array[4];
	*out_matrix[2] = (scalar_t) in_array[8];
	*out_matrix[3] = 0;
	*out_matrix[4] = (scalar_t) in_array[1];
	*out_matrix[5] = (scalar_t) in_array[5];
	*out_matrix[6] = (scalar_t) in_array[9];
	*out_matrix[7] = 0;
	*out_matrix[8] = (scalar_t) in_array[2];
	*out_matrix[9] = (scalar_t) in_array[6];
	*out_matrix[10] = (scalar_t) in_array[10];
	*out_matrix[11] = 0;

}

void Camera::set_camera_type(const TEXEngine::Asset::TEXENG_CAMERA_TYPE cam_type)
{
	m_camera_type = cam_type;
}

const TEXENG_CAMERA_TYPE Camera::get_camera_type()
{
	return m_camera_type;
}

void Camera::move_forward(const float amount)
{
	if(m_camera_type != TEXENG_CAMERA_FPS)
	{
		return;
	}

	if (m_view_dir_changed) 
	{
		get_view_direction();
	}

	Vector3f vMoveVector = Vector3f(0.0f, 0.0f, 0.0f);
	Vector3f vForwardVec = m_forward;

	vMoveVector.x = vForwardVec.x * -amount;
	vMoveVector.y = vForwardVec.y * -amount;
	vMoveVector.z = vForwardVec.z * -amount;

	float fCamPosX = (float)m_position.x;
	float fCamPosY = (float)m_position.y;
	float fCamPosZ = (float)m_position.z;

	fCamPosX = fCamPosX + (float)vMoveVector.x;
	fCamPosY = fCamPosY + (float)vMoveVector.y;
	fCamPosZ = fCamPosZ + (float)vMoveVector.z;

	//Set camera position
	 m_position.x = fCamPosX; 
	 m_position.y = fCamPosY;
	 m_position.z = fCamPosZ;
}
void Camera::get_view_direction()
{
	Vector3f vStep1, vStep2, vForwardVec;
	float PI = 3.14159f;
	float fPitch = (float)m_rotation.x;
	float fYaw = (float)m_rotation.y; 
	//Rotate around Y-axis:
	vStep1.x = cosf( (fYaw + 90.0f) * (PI/180.0f));
	vStep1.z = -sinf( (fYaw + 90.0f) * (PI/180.0f));
	//Rotate around X-axis:
	float fCosX = cosf(fPitch * (PI/180.0f));
	vStep2.x = vStep1.x * fCosX;
	vStep2.z = vStep1.z * fCosX;
	vStep2.y = sinf(fPitch * (PI/180.0f));
	//Rotation around Z-axis not yet implemented, so:
	//m_ViewDir = Step2;
	 vForwardVec = vStep2;
	 //
	m_forward = vForwardVec;
}
void Camera::strafe(const float amount)
{
	if(m_camera_type != TEXENG_CAMERA_FPS)
	{
		return;
	}

	if (m_view_dir_changed) 
	{
		get_view_direction();
	}
	Vector3f vMoveVector;
	Vector3f vForwardVec = m_forward;
	vMoveVector.z = - vForwardVec.x * -amount;
	vMoveVector.y = 0.0;
	vMoveVector.x = vForwardVec.z * -amount;
	
	float fCamPosX = (float)m_position.x;
	float fCamPosY = (float)m_position.y;
	float fCamPosZ = (float)m_position.z;
	fCamPosX = fCamPosX + (float)vMoveVector.x;
	fCamPosY = fCamPosY + (float)vMoveVector.y;
	fCamPosZ = fCamPosZ + (float)vMoveVector.z;

	//Set camera position
	 m_position.x = fCamPosX; 
	 m_position.y = fCamPosY;
	 m_position.z = fCamPosZ;
}

void Camera::set_yaw(const float angle)
{
	if(m_camera_type != TEXENG_CAMERA_FPS)
	{
		return;
	}

	float fYaw = (float)m_rotation.y;
	fYaw += angle;
	m_rotation.y = fYaw;
	m_view_dir_changed = true;
}
void Camera::set_pitch(const float angle)
{
	if(m_camera_type != TEXENG_CAMERA_FPS)
	{
		return;
	}

	float fPitch = (float)m_rotation.x;
	fPitch += angle;
	//m_pCameraNode->RotateIncremental(fAngle, 0.0f, 0.0f);
	m_rotation.x = fPitch;
	m_view_dir_changed = true;
}



void Camera::update_chase_camera()
{

	Vector3f target_up = Vector3f(0.0f, 1.0f, 0.0f);
	// Calculate the new axis;
	Vector3f rz = - m_target_velocity.normalized(); 
	Vector3f rx = -cross(rz, target_up).normalized();
	Vector3f ry = -cross(rx, rz).normalized();

	m_view_matrix.load_identity();

	m_view_matrix *= Matrix4x4f(
		rx.x, rx.y, rx.z, 0.0f,
		ry.x, ry.y, ry.z, 0.0f,
		rz.x, rz.y, rz.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	m_view_matrix.translate(m_target+m_position);
	m_view_matrix.transpose();

	Vector3f eye_position = m_chase_cam_position;/*+ (target_pos - m_position)*/;
	Vector4f temp_pos = Vector4f(eye_position.x, eye_position.y, eye_position.z, 1.0f);
	temp_pos  = m_view_matrix * temp_pos;

	if(Vector2f(m_target_velocity.x, m_target_velocity.z).length() < 0.1f)
	{
		temp_pos.y  = m_target.y + m_chase_cam_position.y;
	}

	//Vector3f target_pos  = m_position + m_target;
	Vector3f lookAt = m_target; //+ m_lookat_offset; 
#ifdef _WIN32
	 gluLookAt(temp_pos.x+ lookAt.x, temp_pos.y+ lookAt.y, temp_pos.z+ lookAt.z,
			   lookAt.x, lookAt.y, lookAt.z,
			   m_up.x, m_up.y, m_up.z);
#endif //_WIN32

	m_position.x = temp_pos.x + lookAt.x;
	m_position.y = temp_pos.y + lookAt.y;
	m_position.z = temp_pos.z + lookAt.z;
}


	} /* namespace Asset */
} /* Namespace TEXEngine */