#ifndef TEXENGINE_SHADER_HPP_INCLUDED
#define TEXENGINE_SHADER_HPP_INCLUDED

#include <map>
#include <string>
#include <vector>
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include "../renderer/shaderParameter.hpp"
#include "../util/utilenum.h"
#include "../math/matrix.hpp"
#include "assetEnum.h"
#include "shaderManager.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "material.hpp"

using TEXEngine::Math::Matrix4x4f;
using TEXEngine::Math::Vector2f;
using TEXEngine::Math::Vector3f;
using TEXEngine::Math::Vector4f;

using TEXEngine::Core::ShaderParameter;
using TEXEngine::Asset::Camera;
using TEXEngine::Asset::Light;
using TEXEngine::Asset::Material;
using TEXEngine::Util::TEXENG_PARAMETER_TYPE;
using TEXEngine::Util::TEXENG_MATRIX_TRANSFORM_TYPE;
using TEXEngine::Util::TEXENG_MATRIX_TYPE;

namespace TEXEngine 
{

	namespace Core 
	{
		class AssetManager; /* Forward declaration */
		class ShaderManager; /* Forward declaration */
	}

	using TEXEngine::Core::ShaderManager;

namespace Asset {

class Shader
{
	friend class ShaderManager;
	friend class AssetManager;
public:
	void bind_programs();
	void unbind_programs();
	void bind_vertex_program();
	void bind_fragment_program();
	void unbind_vertex_program();
	void unbind_fragment_program();
	void vertex_program_parameter_applying_complete(); /*Must be called after the completion fo applying vertex program parameters*/
	void fragment_program_paramter_applying_complete(); /*Must be called after the completion fo applying fragment program parameters*/
	//when initialsing the shader apply vertex and fragment program file names
	int init_shader_programs(const char* vp_file_name, const char* fp_file_name);

protected:
	//Cg shader programs encapsulation
	struct Shader_Programs
	{
		CGprogram m_vertex_program;
		CGprogram m_fragment_program;
		string m_vertex_program_filename;
		string m_fragment_program_filename;
	};

	Shader();
	~Shader();
	
	void deinit();
	int load_shader_programs();

	//Must be called after setting all the neccessary parameters
	void update_shader_parameters();

	//Default shader parameters
	ShaderParameter m_projection_mat_param;
	ShaderParameter m_transform_mat_param;
	ShaderParameter m_model_view_param;
	ShaderParameter m_model_view_projection_param;
	ShaderParameter m_camera_view_mat_param;
	ShaderParameter m_camera_position_param;
	ShaderParameter m_texture_color_param; //diffuse map
	ShaderParameter m_texture_normal_map_param; // normal map
	ShaderParameter m_texture_normal_map_two_param; // normal map
	ShaderParameter m_texture_height_map_param; // height map
	ShaderParameter m_texture_blend_map_param; //Blens map for multi texturing
	ShaderParameter m_texture_color_coord_offset_param;
	ShaderParameter m_texture_height_map_coord_offset_param;
	ShaderParameter m_texture_normal_map_coord_offset_param;
	ShaderParameter m_texture_normal_map_two_coord_offset_param;
	ShaderParameter m_texture_blend_map_coord_offset_param;

	//lights
	//Light one
	ShaderParameter m_light_one_position_param;
	ShaderParameter m_light_one_ambient_param;
	ShaderParameter m_light_one_diffuse_param;
	ShaderParameter m_light_one_specular_param;
	//light two
	ShaderParameter m_light_two_position_param;
	ShaderParameter m_light_two_ambient_param;
	ShaderParameter m_light_two_diffuse_param;
	ShaderParameter m_light_two_specular_param;
	//light three
	ShaderParameter m_light_three_position_param;
	ShaderParameter m_light_three_ambient_param;
	ShaderParameter m_light_three_diffuse_param;
	ShaderParameter m_light_three_specular_param;
	//light four
	ShaderParameter m_light_four_position_param;
	ShaderParameter m_light_four_ambient_param;
	ShaderParameter m_light_four_diffuse_param;
	ShaderParameter m_light_four_specular_param;

	//materials
	ShaderParameter m_material_diffuse_param;
	ShaderParameter m_material_ambient_param;
	ShaderParameter m_material_specular_param;
	ShaderParameter m_material_shininess_param;




public:
	//Passing in default parameters
	void init_default_parameters();
	void update_projection_matrix();
	void update_model_view_matrix();
	void update_model_view_projection_matrix();
	void update_camera(Camera* pcamera);
	void update_transform_matrix(const Matrix4x4f& matrix);
	void update_color_texture(Texture* ptexture);  //diffuse map
	void update_normal_map_texture(Texture* ptexture); // normal map
	void update_normal_map_two_texture(Texture* ptexture); // normal map two
	void update_height_map_texture(Texture* ptexture); // height map
	void update_blend_map_texture(Texture* ptexture); //Blend map
	void update_light_one(Light* plight);
	void update_light_two(Light* plight);
	void update_light_three(Light* plight);
	void update_light_four(Light* plight);
	void update_material(Material* pmaterial);
	//parameters
	//parameter initialisation
	int init_vertex_parameter(ShaderParameter& parameter, char* name, /*name of parameter in shader*/
							  const TEXENG_PARAMETER_TYPE parameterType); /*parameter type*/ 

	int init_fragment_parameter(ShaderParameter& parameter, char* name, /*name of parameter in shader*/
							    const TEXENG_PARAMETER_TYPE parameterType); /*parameter type*/


	//Setting a texture parameter
	int apply_shader_parameter_texture(const ShaderParameter& parameter, Texture* pTexture);

	//apply the parameter variable as a predefined matrix e.g. modelView matrix in openGl
	int apply_matrix_state_parameter(const ShaderParameter& parameter,
								     const TEXENG_MATRIX_TYPE eMatrix, /*Predefined matrix to set for this parameter*/
								     const TEXENG_MATRIX_TRANSFORM_TYPE eTransform); /*Transformation to apply to matrix*/

	//apply the parameter as a matrix of an array of 16 floats i.e. 4x4 matrix...in column order
	int apply_matrix4f_col(const ShaderParameter& parameter, const  Matrix4x4f& matrix);

	//apply the parameter as a float
	int apply_parameter_single_float(const ShaderParameter& parameter,
								     float value);

	//apply the parameter as a vector2f
	int apply_parameter_vector2f(const ShaderParameter& parameter,
							     const Vector2f& vector2f);
	//apply the parameter as a vector3f
	int apply_parameter_vector3f(const ShaderParameter& parameter,
							     const Vector3f& vector3f);

	//apply the parameter as a vector4f
	int apply_parameter_vector4f(const ShaderParameter& parameter,
							   const Vector4f& vector4f);


protected:
	Shader_Programs m_programs;
	//std::vector<ShaderParameter&> m_list_of_vertex_params;
	//std::vector<ShaderParameter&> m_list_of_fragment_params;
};

	} /* namespace Asset */
} /* Namespace TEXEngine */

#endif /* TEXENGINE_SHADER_HPP_INCLUDED */