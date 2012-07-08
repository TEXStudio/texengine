#ifndef TEXENGINE_SHADERMANAGER_HPP_INCLUDED
#define TEXENGINE_SHADERMANAGER_HPP_INCLUDED

#include "texture.hpp"
#include "shader.hpp"
#include "..\message\TEXEngineInterface.hpp"

using TEXEngine::Math::Matrix4x4f;
using TEXEngine::Math::Vector2f;
using TEXEngine::Math::Vector3f;
using TEXEngine::Math::Vector4f;
using TEXEngine::Core::ShaderParameter;
using TEXEngine::Asset::Texture;

using TEXEngine::Util::TEXENG_PARAMETER_TYPE;
using TEXEngine::Util::TEXENG_MATRIX_TRANSFORM_TYPE;
using TEXEngine::Util::TEXENG_MATRIX_TYPE;

namespace TEXEngine 
{
	namespace Asset 
	{
		class Shader; /* Forward declaration */
	}

using Asset::Shader;
//using TEXEngine::Util::TEXENG_PARAMETER_TYPE;
//using TEXEngine::Util::TEXENG_MATRIX_TRANSFORM_TYPE;
//using TEXEngine::Util::TEXENG_MATRIX_TYPE;

	namespace Core 
	{
		class ShaderManager : public Object
		{
		public:
			~ShaderManager();
			static ShaderManager& get();

			//Initialises the cg shader context
			int initialise();

			//Shaders
			//Returns 0 if there are no errors
			//and returns null on the error string
			int check_cgErrors(const char* operation);

			//Binding shaders
			int bind_shader_vertex_program(Shader* pShader); //Bind vertex program only
			int bind_shader_frag_program(Shader* pShader); //Bind fragment program only
			int bind_shader_vertex_frag_programs(Shader* pShader); //Bind all programs
			int unbind_shader_vertex_program(Shader* pShader);
			int unbind_shader_frag_program(Shader* pShaders);
			int unbind_shader_vertex_frag_program(Shader* pShader);
			int load_shader_programs(Shader* pShader);
			void cleanup_shader_programs(Shader* pShader);

			//Called within shader objecr when setting parameters is completer
			void enable_vertex_profile();
			void enable_fragment_profile();

			///shader parameters

			//Must be called inorder to cleanup the CGParameter handle
			//object otherwise you have memory leakes...
			void cleanup_shader_parameter(ShaderParameter& parameter);

			//initialise shader parameters
			void enable_texture_parameter(const ShaderParameter& parameter); /*Must called before apply the texture for rendering*/
			void disable_texture_parameter(const ShaderParameter& parameter); /*Must be called after applying the texture for rendering*/

			int init_vertex_shader_parameter(Shader* pShader, ShaderParameter& parameter, const char* name, /*name of parameter in shader*/
											const TEXENG_PARAMETER_TYPE parameterType);

			int init_fragment_shader_parameter(Shader* pShader, ShaderParameter& parameter, const char* name, /*name of parameter in shader*/
											   const TEXENG_PARAMETER_TYPE parameterType);

			//Setting a texture parameter
			int apply_shader_parameter_texture(const ShaderParameter& parameter, Texture* pTexture);

			//Set the parameter variable as a predefined matrix e.g. modelView matrix in openGl
			int apply_matrix_state_shader_parameter(const ShaderParameter& parameter,
													const TEXENG_MATRIX_TYPE eMatrix, /*Predefined matrix to set for this parameter*/
													const TEXENG_MATRIX_TRANSFORM_TYPE eTransform); /*Transformation to apply to matrix*/

			//Set the parameter as a matrix of an array of 16 floats i.e. 4x4 matrix...in column order
			int apply_shader_parameter_matrix4f_col(const ShaderParameter& parameter, const Matrix4x4f& matrix);
						    
			//set the parameter as a float
			int apply_shader_parameter_single_float(const ShaderParameter& parameter,
													float value);

			//Set the parameter as a vector2f
			int apply_shader_parameter_vector2f(const ShaderParameter& parameter,
												const Vector2f& vector2f);

			//Set the parameter as a vector3f
			int apply_shader_parameter_vector3f(const ShaderParameter& parameter,
												const Vector3f& vector3f);

			//set the parameter as a vector4f
			int apply_shader_parameter_vector4f(const ShaderParameter& parameter,
												const Vector4f& vector4f);

			void on_event(const unsigned long event_code,
						const Message& parameters);
			
			void setup(); 

		private:
			ShaderManager();
			void release();

			//CG context
			void init_cgContext();
			void deinit_cgContext();
			void cgContext_init_error_checking(const char* operation);

			static ShaderManager m_shader_manager;

#ifdef _WIN32 
			/*With the PC you can load source shaders*/
			void create_source_shader_frag(Shader* pShader);
			void create_source_shader_vert(Shader* pShader);
#endif /*_WIN32*/

			CGcontext m_cg_context;
			CGprofile m_vertex_profile;
			CGprofile m_fragment_profile;
			bool m_bCgInitialised;
			unsigned long m_update_rate;
			
			//matrix conversions
			void matrix_to_1D_array(float* outArray, const Matrix4x4f& matrix);
			void matrix_to_1D_array(float* outArray, const Matrix4x4f* matrix);

			void change_update_rate(const unsigned long update_rate);
		};
	}
}
#endif /* TEXENGINE_SHADERMANAGER_HPP_INCLUDED */