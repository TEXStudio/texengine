#include "../asset/assetManager.hpp"
#include "shader.hpp"
#include "shaderManager.hpp"
#include "../log/logmanager.hpp"

using TEXEngine::Core::AssetManager;
using TEXEngine::Core::ShaderManager;

namespace TEXEngine {
	namespace Asset {

		Shader::Shader()
		{

		}
		Shader::~Shader()
		{
			deinit();
		}


		//Passing in default parameters
		void Shader::init_default_parameters()
		{
			//init vertex parameters
			init_vertex_parameter(m_projection_mat_param, "mat_projection", TEXEngine::Util::TEXENG_PARAMETER_MATRIX4F);
			init_vertex_parameter(m_model_view_param, "mat_model_view", TEXEngine::Util::TEXENG_PARAMETER_MATRIX4F);
			init_vertex_parameter(m_model_view_projection_param, "mat_model_view_projection", TEXEngine::Util::TEXENG_PARAMETER_MATRIX4F);
			init_vertex_parameter(m_transform_mat_param, "mat_transform", TEXEngine::Util::TEXENG_PARAMETER_MATRIX4F);
			init_vertex_parameter(m_camera_view_mat_param, "mat_view", TEXEngine::Util::TEXENG_PARAMETER_MATRIX4F);
			init_vertex_parameter(m_camera_position_param, "cam_position", TEXEngine::Util::TEXENG_PARAMETER_VECTOR3F);

			//Fragment parameters
			//lights
			//light positions
			init_fragment_parameter(m_light_one_position_param, "lit_1_position", TEXEngine::Util::TEXENG_PARAMETER_VECTOR3F);
			init_fragment_parameter(m_light_two_position_param, "lit_2_position", TEXEngine::Util::TEXENG_PARAMETER_VECTOR3F);
			init_fragment_parameter(m_light_three_position_param, "lit_3_position", TEXEngine::Util::TEXENG_PARAMETER_VECTOR3F);
			init_fragment_parameter(m_light_four_position_param, "lit_4_position", TEXEngine::Util::TEXENG_PARAMETER_VECTOR3F);
			init_fragment_parameter(m_light_one_ambient_param, "lit_1_ambient", TEXEngine::Util::TEXENG_PARAMETER_VECTOR4F);
			init_fragment_parameter(m_light_two_ambient_param, "lit_2_ambient", TEXEngine::Util::TEXENG_PARAMETER_VECTOR4F);
			init_fragment_parameter(m_light_three_ambient_param, "lit_3_ambient", TEXEngine::Util::TEXENG_PARAMETER_VECTOR4F);
			init_fragment_parameter(m_light_four_ambient_param, "lit_4_ambient", TEXEngine::Util::TEXENG_PARAMETER_VECTOR4F);
			//
			init_fragment_parameter(m_light_one_diffuse_param, "lit_1_diffuse", TEXEngine::Util::TEXENG_PARAMETER_VECTOR4F);
			init_fragment_parameter(m_light_two_diffuse_param, "lit_2_diffuse", TEXEngine::Util::TEXENG_PARAMETER_VECTOR4F);
			init_fragment_parameter(m_light_three_diffuse_param, "lit_3_diffuse", TEXEngine::Util::TEXENG_PARAMETER_VECTOR4F);
			init_fragment_parameter(m_light_four_diffuse_param, "lit_4_diffuse", TEXEngine::Util::TEXENG_PARAMETER_VECTOR4F);
			//
			init_fragment_parameter(m_light_one_specular_param, "lit_1_specular", TEXEngine::Util::TEXENG_PARAMETER_VECTOR4F);
			init_fragment_parameter(m_light_two_specular_param, "lit_2_specular", TEXEngine::Util::TEXENG_PARAMETER_VECTOR4F);
			init_fragment_parameter(m_light_three_specular_param, "lit_3_specular", TEXEngine::Util::TEXENG_PARAMETER_VECTOR4F);
			init_fragment_parameter(m_light_four_specular_param, "lit_4_specular", TEXEngine::Util::TEXENG_PARAMETER_VECTOR4F);

			//Textures
			init_fragment_parameter(m_texture_color_param, "tex_color", TEXEngine::Util::TEXENG_PARAMETER_TEXTURE);
			init_fragment_parameter(m_texture_normal_map_param, "tex_normal", TEXEngine::Util::TEXENG_PARAMETER_TEXTURE);
			init_fragment_parameter(m_texture_normal_map_two_param, "tex_normal_two", TEXEngine::Util::TEXENG_PARAMETER_TEXTURE);
			init_fragment_parameter(m_texture_height_map_param, "tex_height", TEXEngine::Util::TEXENG_PARAMETER_TEXTURE);
			init_fragment_parameter(m_texture_blend_map_param, "tex_blend", TEXEngine::Util::TEXENG_PARAMETER_TEXTURE);

			//coordinate offsets
			init_fragment_parameter(m_texture_color_coord_offset_param, "tex_color_coord_offset", TEXEngine::Util::TEXENG_PARAMETER_VECTOR2F);
			init_fragment_parameter(m_texture_height_map_coord_offset_param, "tex_height_map_coord_offset", TEXEngine::Util::TEXENG_PARAMETER_VECTOR2F);
			init_fragment_parameter(m_texture_normal_map_coord_offset_param, "tex_normal_map_coord_offset", TEXEngine::Util::TEXENG_PARAMETER_VECTOR2F);
			init_fragment_parameter(m_texture_normal_map_two_coord_offset_param, "tex_normal_map_two_coord_offset", TEXEngine::Util::TEXENG_PARAMETER_VECTOR2F);
			init_fragment_parameter(m_texture_blend_map_coord_offset_param, "tex_blend_map_coord_offset", TEXEngine::Util::TEXENG_PARAMETER_VECTOR2F);

			//Material
			init_fragment_parameter(m_material_shininess_param, "mtl_shininess", TEXEngine::Util::TEXENG_PARAMETER_FLOAT);
			init_fragment_parameter(m_material_diffuse_param, "mtl_diffuse", TEXEngine::Util::TEXENG_PARAMETER_VECTOR4F);
			init_fragment_parameter(m_material_ambient_param, "mtl_ambient", TEXEngine::Util::TEXENG_PARAMETER_VECTOR4F);
			init_fragment_parameter(m_material_specular_param, "mtl_specular", TEXEngine::Util::TEXENG_PARAMETER_VECTOR4F);

		}
		void Shader::update_projection_matrix()
		{
			apply_matrix_state_parameter(m_projection_mat_param,
										 TEXEngine::Util::TEXENG_MATRIX_PROJECTION,
										  TEXEngine::Util::TEXENG_MATRIX_TRANSFORM_IDENTITY);
		}
		void Shader::update_model_view_matrix()
		{
			apply_matrix_state_parameter(m_model_view_param,
				TEXEngine::Util::TEXENG_MATRIX_MODELVIEW,
										  TEXEngine::Util::TEXENG_MATRIX_TRANSFORM_IDENTITY);
		}
		void Shader::update_model_view_projection_matrix()
		{
			apply_matrix_state_parameter(m_model_view_projection_param,
										 TEXEngine::Util::TEXENG_MATRIX_MODELVIEW_PROJECTION,
										 TEXEngine::Util::TEXENG_MATRIX_TRANSFORM_IDENTITY);
		}

		void Shader::update_camera(Camera* pcamera)
		{
			//Apply camera position
			Vector3f cam_pos = pcamera->position();
			apply_parameter_vector3f(m_camera_position_param, cam_pos);

			//Apply view matrix
			Matrix4x4f view_mat =  pcamera->view_matrix();
			apply_matrix4f_col(m_camera_view_mat_param, view_mat);
		}
		void Shader::update_transform_matrix(const Matrix4x4f& matrix)
		{
			apply_matrix4f_col(m_transform_mat_param, matrix);
		}
		void Shader::update_color_texture(Texture* ptexture)  //diffuse map
		{
			apply_shader_parameter_texture(m_texture_color_param, ptexture);
			apply_parameter_vector2f(m_texture_color_coord_offset_param, ptexture->offset());
			ShaderManager::get().enable_texture_parameter(m_texture_color_param);
		}
		void Shader::update_normal_map_texture(Texture* ptexture) // normal map
		{
			apply_shader_parameter_texture(m_texture_normal_map_param, ptexture);
			apply_parameter_vector2f(m_texture_normal_map_coord_offset_param, ptexture->offset());
			ShaderManager::get().enable_texture_parameter(m_texture_normal_map_param);
		}
		void Shader::update_normal_map_two_texture(Texture* ptexture) // normal map two
		{
			apply_shader_parameter_texture(m_texture_normal_map_two_param, ptexture);
			apply_parameter_vector2f(m_texture_normal_map_two_coord_offset_param, ptexture->offset());
			ShaderManager::get().enable_texture_parameter(m_texture_normal_map_two_param);
		}
		void Shader::update_height_map_texture(Texture* ptexture) // height map
		{
			apply_shader_parameter_texture(m_texture_height_map_param, ptexture);
			apply_parameter_vector2f(m_texture_height_map_coord_offset_param, ptexture->offset());
			ShaderManager::get().enable_texture_parameter(m_texture_height_map_param);
		}
		void Shader::update_blend_map_texture(Texture* ptexture) //Blend map
		{
			apply_shader_parameter_texture(m_texture_blend_map_param, ptexture);
			apply_parameter_vector2f(m_texture_blend_map_coord_offset_param, ptexture->offset());
			ShaderManager::get().enable_texture_parameter(m_texture_blend_map_param);
		}

		void Shader::update_light_one(Light* plight)
		{
			Vector3f light_pos = plight->position();
			Vector4f light_diffuse = Vector4f(plight->diffuse().red, plight->diffuse().green,
											  plight->diffuse().blue, plight->diffuse().alpha);

			Vector4f light_ambient = Vector4f(plight->ambient().red, plight->ambient().green,
											  plight->ambient().blue, plight->ambient().alpha);

			Vector4f light_specular = Vector4f(plight->specular().red, plight->specular().green,
											   plight->specular().blue, plight->specular().alpha); 

			//Apply shader parameters
			apply_parameter_vector3f(m_light_one_position_param, light_pos);
			apply_parameter_vector4f(m_light_one_ambient_param, light_ambient);
			apply_parameter_vector4f(m_light_one_diffuse_param, light_diffuse);
			apply_parameter_vector4f(m_light_one_specular_param, light_specular);

		}
		void Shader::update_light_two(Light* plight)
		{
			Vector3f light_pos = plight->position();
			Vector4f light_diffuse = Vector4f(plight->diffuse().red, plight->diffuse().green,
											  plight->diffuse().blue, plight->diffuse().alpha);

			Vector4f light_ambient = Vector4f(plight->ambient().red, plight->ambient().green,
											  plight->ambient().blue, plight->ambient().alpha);

			Vector4f light_specular = Vector4f(plight->specular().red, plight->specular().green,
											   plight->specular().blue, plight->specular().alpha); 

			//Apply shader parameters
			apply_parameter_vector3f(m_light_two_position_param, light_pos);
			apply_parameter_vector4f(m_light_two_ambient_param, light_ambient);
			apply_parameter_vector4f(m_light_two_diffuse_param, light_diffuse);
			apply_parameter_vector4f(m_light_two_specular_param, light_specular);
		}
		void Shader::update_light_three(Light* plight)
		{
			Vector3f light_pos = plight->position();
			Vector4f light_diffuse = Vector4f(plight->diffuse().red, plight->diffuse().green,
											  plight->diffuse().blue, plight->diffuse().alpha);

			Vector4f light_ambient = Vector4f(plight->ambient().red, plight->ambient().green,
											  plight->ambient().blue, plight->ambient().alpha);

			Vector4f light_specular = Vector4f(plight->specular().red, plight->specular().green,
											   plight->specular().blue, plight->specular().alpha); 

			//Apply shader parameters
			apply_parameter_vector3f(m_light_three_position_param, light_pos);
			apply_parameter_vector4f(m_light_three_ambient_param, light_ambient);
			apply_parameter_vector4f(m_light_three_diffuse_param, light_diffuse);
			apply_parameter_vector4f(m_light_three_specular_param, light_specular);
		}
		void Shader::update_light_four(Light* plight)
		{
			Vector3f light_pos = plight->position();
			Vector4f light_diffuse = Vector4f(plight->diffuse().red, plight->diffuse().green,
											  plight->diffuse().blue, plight->diffuse().alpha);

			Vector4f light_ambient = Vector4f(plight->ambient().red, plight->ambient().green,
											  plight->ambient().blue, plight->ambient().alpha);

			Vector4f light_specular = Vector4f(plight->specular().red, plight->specular().green,
											   plight->specular().blue, plight->specular().alpha); 

			//Apply shader parameters
			apply_parameter_vector3f(m_light_four_position_param, light_pos);
			apply_parameter_vector4f(m_light_four_ambient_param, light_ambient);
			apply_parameter_vector4f(m_light_four_diffuse_param, light_diffuse);
			apply_parameter_vector4f(m_light_four_specular_param, light_specular);
		}

		void Shader::update_material(Material* pmaterial)
		{
			Vector4f ambient = Vector4f(pmaterial->ambient().red, pmaterial->ambient().green,
									    pmaterial->ambient().blue, pmaterial->ambient().alpha);

			Vector4f diffuse = Vector4f(pmaterial->diffuse().red, pmaterial->diffuse().green,
									    pmaterial->diffuse().blue, pmaterial->diffuse().alpha);

			Vector4f specular = Vector4f(pmaterial->specular().red, pmaterial->specular().green,
									     pmaterial->specular().blue, pmaterial->specular().alpha);

			float shininess =  pmaterial->shininess();

			apply_parameter_single_float(m_material_shininess_param, shininess);
			apply_parameter_vector4f(m_material_ambient_param, ambient);
			apply_parameter_vector4f(m_material_diffuse_param, diffuse);
			apply_parameter_vector4f(m_material_specular_param, specular);
		}
	
		//when initialsing the shader apply vertex and fragment program file names
		int Shader::init_shader_programs(const char* vp_file_name, const char* fp_file_name)
		{
			m_programs.m_vertex_program_filename = string(vp_file_name);
			m_programs.m_fragment_program_filename = string(fp_file_name);

			return 0;
		}
		void Shader::bind_programs()
		{
			ShaderManager::get().bind_shader_vertex_frag_programs(this);
			ShaderManager::get().bind_shader_frag_program(this);
		}
		void Shader::unbind_programs()
		{
			ShaderManager::get().unbind_shader_vertex_program(this);
			ShaderManager::get().unbind_shader_frag_program(this);
		}
		void Shader::bind_vertex_program()
		{
			ShaderManager::get().bind_shader_vertex_program(this);
		}
		
		void Shader::bind_fragment_program()
		{
			ShaderManager::get().bind_shader_frag_program(this);
		}

		void Shader::unbind_fragment_program()
		{
			ShaderManager::get().unbind_shader_frag_program(this);
		}

		void Shader::unbind_vertex_program()
		{
			ShaderManager::get().unbind_shader_vertex_program(this);
		}
		void Shader::deinit()
		{
			ShaderManager::get().cleanup_shader_programs(this);
		}
		int Shader::load_shader_programs()
		{
			int iErrorStatus  = ShaderManager::get().load_shader_programs(this);

			return iErrorStatus;
		}

		/*Must be called after the completion fo applying vertex program parameters*/
		void Shader::vertex_program_parameter_applying_complete()
		{
			 ShaderManager::get().enable_vertex_profile();
		}

		/*Must be called after the completion fo applying fragment program parameters*/
		void Shader::fragment_program_paramter_applying_complete()
		{
			 ShaderManager::get().enable_fragment_profile();
		}

		//Must be called after setting all the neccessary parameters
		void Shader::update_shader_parameters()
		{
#ifdef _WIN32
			cgUpdateProgramParameters(m_programs.m_vertex_program);
			cgUpdateProgramParameters(m_programs.m_fragment_program);
#endif //_WIN32
		}

		//parameters
		int Shader::init_vertex_parameter(ShaderParameter& parameter, char* name, /*name of parameter in shader*/
										  const TEXENG_PARAMETER_TYPE parameterType)
		{
			int iStatus = ShaderManager::get().init_vertex_shader_parameter(this, parameter, name, parameterType);

			return iStatus;
		}
		int Shader::init_fragment_parameter(ShaderParameter& parameter, char* name, /*name of parameter in shader*/
											const TEXENG_PARAMETER_TYPE parameterType)
		{
			int iStatus = ShaderManager::get().init_fragment_shader_parameter(this, parameter, name, parameterType);
			return iStatus;
		}

		int Shader::apply_shader_parameter_texture(const ShaderParameter& parameter, Texture* pTexture)
		{
			int iStatus = ShaderManager::get().apply_shader_parameter_texture(parameter, pTexture);

			return iStatus;
		}

		//apply the parameter variable as a predefined matrix e.g. modelView matrix in openGl
		int Shader::apply_matrix_state_parameter(const ShaderParameter& parameter,
												 const TEXENG_MATRIX_TYPE eMatrix, /*Predefined matrix to set for this parameter*/
												 const TEXENG_MATRIX_TRANSFORM_TYPE eTransform) /*Transformation to apply to matrix*/
		{
			ShaderManager::get().apply_matrix_state_shader_parameter(parameter, eMatrix, eTransform);
			return 0;
		}

		//apply the parameter as a matrix of an array of 16 floats i.e. 4x4 matrix...in column order
		int Shader::apply_matrix4f_col(const ShaderParameter& parameter, const  Matrix4x4f& matrix)
		{
			ShaderManager::get().apply_shader_parameter_matrix4f_col(parameter, matrix);

			return 0;
		}

		//apply the parameter as a float
		int Shader::apply_parameter_single_float(const ShaderParameter& parameter,
										         float value)
		{
			ShaderManager::get().apply_shader_parameter_single_float(parameter, value);
			return 0;
		}

		//apply the parameter as a vector2f
		int Shader::apply_parameter_vector2f(const ShaderParameter& parameter,
							                 const Vector2f& vector2f)
		{
			ShaderManager::get().apply_shader_parameter_vector2f(parameter, vector2f);
			return 0;
		}
		//apply the parameter as a vector3f
		int Shader::apply_parameter_vector3f(const ShaderParameter& parameter,
									         const Vector3f& vector3f)
		{
			ShaderManager::get().apply_shader_parameter_vector3f(parameter, vector3f);
			return 0;
		}

		//apply the parameter as a vector4f
		int Shader::apply_parameter_vector4f(const ShaderParameter& parameter,
								           const Vector4f& vector4f)
		{
			ShaderManager::get().apply_shader_parameter_vector4f(parameter, vector4f);
			return 0;
		}

	} /* namespace Asset */
} /* Namespace TEXEngine */