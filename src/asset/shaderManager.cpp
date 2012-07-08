#include "shaderManager.hpp"
#include "../util/utilenum.h"
#include "../log/logManager.hpp"

namespace TEXEngine 
{
using TEXEngine::Util::TEXENG_PARAMETER_TYPE;
using TEXEngine::Util::TEXENG_MATRIX_TRANSFORM_TYPE;
using TEXEngine::Util::TEXENG_MATRIX_TYPE;

	namespace Core 
	{
		ShaderManager ShaderManager::m_shader_manager;

		ShaderManager::ShaderManager() : Object(7)
		{

		}

		ShaderManager::~ShaderManager()
		{
			release();
		}

		void ShaderManager::release()
		{
			//sutdown cg context
			cgDestroyContext(m_cg_context);
			m_cg_context= 0;
		}

		ShaderManager& ShaderManager::get()
		{
			return m_shader_manager;
		}

		int ShaderManager::initialise()
		{
			init_cgContext();

			return 0;
		}

		//Shaders
		//Returns 0 if there are no errors
		//and returns null on the error string
		int ShaderManager::check_cgErrors(const char* operation)
		{
			CGerror cgError;
			const char* outError = cgGetLastErrorString(&cgError);

			//cg error has occured
			if(cgError != CG_NO_ERROR)
			{

				//Check cg shader compile error
				if(cgError == CG_COMPILER_ERROR)
				{
					const char* compileError = cgGetLastListing(m_cg_context);
					LogManager::get().log_error("%s", operation);
					LogManager::get().log_error("%s", compileError);

					return 1;
				}

				LogManager::get().log_error("%s", operation);
				LogManager::get().log_error("%i", cgError);

				return 1;
			}

			return 0;
		}

		//Binding shaders
		int ShaderManager::bind_shader_vertex_program(Shader* pShader) //Bind vertex program only
		{
			cgGLBindProgram(pShader->m_programs.m_vertex_program);
			cgGLEnableProfile(m_vertex_profile);

			return 0;
		}
		int ShaderManager::bind_shader_frag_program(Shader* pShader) //Bind fragment program only
		{
			cgGLBindProgram(pShader->m_programs.m_fragment_program);
			cgGLEnableProfile(m_fragment_profile);

			return 0;
		}
		int ShaderManager::bind_shader_vertex_frag_programs(Shader* pShader) //Bind all programs
		{
			cgGLBindProgram(pShader->m_programs.m_vertex_program);
			cgGLEnableProfile(m_vertex_profile);

			cgGLBindProgram(pShader->m_programs.m_fragment_program);
			cgGLEnableProfile(m_fragment_profile);

			return 0;
		}
		int ShaderManager::unbind_shader_vertex_program(Shader* pShader)
		{
			cgGLBindProgram(pShader->m_programs.m_vertex_program);
			cgGLDisableProfile(m_vertex_profile);

			cgGLBindProgram(pShader->m_programs.m_fragment_program);
			cgGLDisableProfile(m_fragment_profile);

			return 0;
		}
		int ShaderManager::unbind_shader_frag_program(Shader* pShaders)
		{
			cgGLDisableProfile(m_fragment_profile);

			return 0;
		}
		int ShaderManager::unbind_shader_vertex_frag_program(Shader* pShader)
		{
			cgGLDisableProfile(m_vertex_profile);

			return 0;
		}

		//Called within shader objecr when setting parameters is completer
		void ShaderManager::enable_vertex_profile()
		{
			cgGLEnableProfile(m_vertex_profile);
		}
		void ShaderManager::enable_fragment_profile()
		{
			cgGLEnableProfile(m_fragment_profile);
		}

		//CG context
		void ShaderManager::init_cgContext()
		{
			//Create CG context
			m_cg_context = cgCreateContext();
			cgContext_init_error_checking("Creating cg context"); //Check for any errors
			cgGLSetDebugMode( CG_FALSE );

		#ifdef _WIN32
			cgSetParameterSettingMode(m_cg_context, CG_DEFERRED_PARAMETER_SETTING);
		#endif //_WIN32

			//Vertex profile
			m_vertex_profile = cgGLGetLatestProfile(CG_GL_VERTEX);
			cgGLSetOptimalOptions(m_vertex_profile);
			cgContext_init_error_checking("selecting vertex profile"); //Check for any errors

			//Fragment profile
			m_fragment_profile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
			cgGLSetOptimalOptions(m_fragment_profile);
			cgContext_init_error_checking("selecting fragment profile"); //Check for errors
		}

		void ShaderManager::deinit_cgContext()
		{
			cgDestroyContext(m_cg_context);
			 m_cg_context = 0;
		}

		//Check errors when initialising the CGcontext
		void ShaderManager::cgContext_init_error_checking(const char* operation)
		{
			char* errors = 0;
			int iStatus = check_cgErrors(operation);
			//Error is detected
			if(iStatus > 0)
			{
				m_bCgInitialised = false;

				LogManager::get().log_error("%s", errors);

			}
			else
			{
				m_bCgInitialised = true;
			}
		}

		int ShaderManager::load_shader_programs(Shader* pShader)
		{
			int iErrorStatus = 0;

			//cg context is not initialised
			if(!m_bCgInitialised)
			{
				return 1;
			}
			
/******************************LOAD VERTEX PROGRAM******************************************************/
	
	#ifdef _WIN32
			cgGLSetOptimalOptions(m_vertex_profile);
			create_source_shader_vert(pShader);
	#endif //_WIN32

	

			iErrorStatus = check_cgErrors("creating vertex program from file");
			if(iErrorStatus > 0)
			{
				//Error occured
				return 1;
			}

			cgGLLoadProgram(pShader->m_programs.m_vertex_program); //Load vertex program
			iErrorStatus = check_cgErrors("loading vertex program"); 
			if(iErrorStatus > 0)
			{
				//Error occured
				return 1;
			}

	/****************************************************************************************/

	/*********************************LOAD FRAGMENT PROGRAM**************************************/

	#ifdef _WIN32
			cgGLSetOptimalOptions(m_fragment_profile);
			create_source_shader_frag(pShader);
	#endif //_WIN32

			iErrorStatus = check_cgErrors("creating vertex program from file");
			if(iErrorStatus > 0)
			{
				//Error occured
				return 1;
			}
			
			if (pShader)
				cgGLLoadProgram(pShader->m_programs.m_fragment_program); //Load 

			iErrorStatus = check_cgErrors("loading fragment program");
			if(iErrorStatus > 0)
			{
				//Error occured
				return 1;
			}

	/***************************************************************************************/	
			return 0;
		}
		void ShaderManager::cleanup_shader_programs(Shader* pShader)
		{
			cgDestroyProgram(pShader->m_programs.m_vertex_program);
			cgDestroyProgram(pShader->m_programs.m_fragment_program);
		}

	#ifdef _WIN32 
		/*With the PC you can load source shaders*/
		void ShaderManager::create_source_shader_frag(Shader* pShader)
		{
				pShader->m_programs.m_fragment_program = cgCreateProgramFromFile(
												m_cg_context, /* Cg runtime context */
												CG_SOURCE,/* Program in human-readable form */
												pShader->m_programs.m_fragment_program_filename.c_str(),/* Name of file containing program */
												m_fragment_profile, /* Profile: OpenGL ARB fragment program */
												NULL, /* Entry function name  will always be main*/
												NULL);/* No extra compiler options */
		}
		void ShaderManager::create_source_shader_vert(Shader *pShader)
		{
			pShader->m_programs.m_vertex_program = cgCreateProgramFromFile(
												m_cg_context, /* Cg runtime context */
												CG_SOURCE,/* Program in human-readable form */
												pShader->m_programs.m_vertex_program_filename.c_str(),/* Name of file containing program */
												m_vertex_profile, /* Profile: OpenGL ARB vertex program */
												NULL, /* Entry function name  will always be main*/
												NULL);/* No extra compiler options*/
		}
	#endif //_WIN32 

		///applying shader parameters
		//Set the parameter variable as a predefined matrix e.g. modelView matrix in openGl
		int ShaderManager::apply_matrix_state_shader_parameter(const ShaderParameter& parameter,
															  const TEXENG_MATRIX_TYPE eMatrix, /*Predefined matrix to set for this parameter*/
															  const TEXENG_MATRIX_TRANSFORM_TYPE eTransform) /*Transformation to apply to matrix*/
		{
	
			CGGLenum matrixType = CG_GL_MODELVIEW_MATRIX;
			CGGLenum transformation = CG_GL_MATRIX_IDENTITY;
			//Matrix to apply
			switch(eMatrix)
			{
			case TEXEngine::Util::TEXENG_MATRIX_MODELVIEW:
				matrixType = CG_GL_MODELVIEW_MATRIX;
				break;
			case TEXEngine::Util::TEXENG_MATRIX_PROJECTION:
				matrixType = CG_GL_PROJECTION_MATRIX;
				break;
			case TEXEngine::Util::TEXENG_MATRIX_MODELVIEW_PROJECTION:
				matrixType = CG_GL_MODELVIEW_PROJECTION_MATRIX;
				break;
			}

			//Transformation to apply to matrix
			switch(eTransform)
			{
			case TEXEngine::Util::TEXENG_MATRIX_TRANSFORM_IDENTITY:
				transformation = CG_GL_MATRIX_IDENTITY;
				break;
			case TEXEngine::Util::TEXENG_MATRIX_TRANSFORM_TRANSPOSE:
				transformation = CG_GL_MATRIX_TRANSPOSE;
				break;
			case TEXEngine::Util::TEXENG_MATRIX_TRANSFORM_INVERSE:
				transformation = CG_GL_MATRIX_INVERSE;
			case TEXEngine::Util::TEXENG_MATRIX_TRANSFORM_INVERSE_TRANSPOSE:
				transformation = CG_GL_MATRIX_INVERSE_TRANSPOSE;
				break;
			}

			//Set the shader parameter handle value as this predefined matrix
			cgGLSetStateMatrixParameter(parameter.m_cg_parameter_handle,
										matrixType,
										transformation);

			return 0;
		}

		//Shader parameters
		void ShaderManager::cleanup_shader_parameter(ShaderParameter& parameter)
		{
			cgDestroyParameter(parameter.m_cg_parameter_handle);
			parameter.m_cg_parameter_handle = 0;
		}

		void ShaderManager::enable_texture_parameter(const ShaderParameter& parameter)
		{
			cgGLEnableTextureParameter(parameter.m_cg_parameter_handle);
		}
		void ShaderManager::disable_texture_parameter(const ShaderParameter& parameter)
		{
			cgGLDisableTextureParameter(parameter.m_cg_parameter_handle);
		}

		//initialise shader parameters
		int ShaderManager::init_vertex_shader_parameter(Shader* pShader, ShaderParameter& parameter, const char* name, /*name of parameter in shader*/
													   const TEXENG_PARAMETER_TYPE parameterType)
		{
			parameter.m_parameter_name = string(name);
			parameter.m_eType = parameterType;
			//Obtain parameter handle from shader
			parameter.m_cg_parameter_handle = cgGetNamedParameter(pShader->m_programs.m_vertex_program,
																  parameter.m_parameter_name.c_str());
			//TODO...error checking here

			return 0;
		}

		int ShaderManager::init_fragment_shader_parameter(Shader* pShader, ShaderParameter& parameter, const char* name, /*name of parameter in shader*/
														 const TEXENG_PARAMETER_TYPE parameterType)
		{
			parameter.m_parameter_name = string(name);
			parameter.m_eType = parameterType;
			//Obtain parameter handle from shader
			parameter.m_cg_parameter_handle = cgGetNamedParameter(pShader->m_programs.m_fragment_program,
																  parameter.m_parameter_name.c_str());
			//TODO...error checking here

			return 0;
		}

		//Setting a texture parameter
		int ShaderManager::apply_shader_parameter_texture(const ShaderParameter& parameter, Texture* pTexture)
		{
			cgGLSetTextureParameter(parameter.m_cg_parameter_handle, pTexture->id());

			return 0;
		}

		//Set the parameter as a matrix of an array of 16 floats i.e. 4x4 matrix...in column order
		int ShaderManager::apply_shader_parameter_matrix4f_col(const ShaderParameter& parameter, const Matrix4x4f& matrix)
		{
			float temp_mat_array[16];
			matrix_to_1D_array(temp_mat_array, matrix);

			cgGLSetMatrixParameterfc(parameter.m_cg_parameter_handle, temp_mat_array);

			return 0;
		}
						    
		//set the parameter as a float
		int ShaderManager::apply_shader_parameter_single_float(const ShaderParameter& parameter,
															  float value)
		{
			cgSetParameter1f(parameter.m_cg_parameter_handle, value);
			return 0;
		}

		//Set the parameter as a vector2f
		int ShaderManager::apply_shader_parameter_vector2f(const ShaderParameter& parameter,
														  const Vector2f& vector2f)
		{
			cgSetParameter2f(parameter.m_cg_parameter_handle, (float)vector2f.x, (float)vector2f.y);
			return 0;
		}

		//Set the parameter as a vector3f
		int ShaderManager::apply_shader_parameter_vector3f(const ShaderParameter& parameter,
														  const Vector3f& vector3f)
		{
			cgSetParameter3f(parameter.m_cg_parameter_handle, (float)vector3f.x, (float)vector3f.y, (float)vector3f.z);

			return 0;
		}

		//set the parameter as a vector4f
		int ShaderManager::apply_shader_parameter_vector4f(const ShaderParameter& parameter,
														  const Vector4f& vector4f)
		{
			cgSetParameter4f(parameter.m_cg_parameter_handle, (float)vector4f.x, (float)vector4f.y, 
							(float)vector4f.z, (float)vector4f.w);
			return 0;
		}

		void ShaderManager::matrix_to_1D_array(float* outArray, const Matrix4x4f& matrix)
		{
			Matrix4x4f mat = matrix;
			for(int i = 0; i < 16; i++)
			{
				outArray[i] = (float)*mat[i];
			}
		}
		void ShaderManager::matrix_to_1D_array(float* outArray, const Matrix4x4f* matrix)
		{
			Matrix4x4f mat = *matrix;
			for(int i = 0; i < 16; i++)
			{
				outArray[i] = (float)*mat[i];
			}
		}

		void ShaderManager::on_event(const unsigned long event_code, const Message& parameters)
		{
			/*if (event_code == m_update_rate)
			{
				// do update stuff
			}
			else*/ if (event_code == INITIALISE )
				initialise();
			/*else if (event_code == EXIT )
				release();
			else if ( event_code == SHADER_MANAGER_CHANGE_UPDATE_RATE)
				change_update_rate((unsigned long)parameters.get_integral_data(2));*/
		}

		void ShaderManager::setup()
		{
			register_event(INITIALISE);
			//register_event(EXIT);
			//register_event(SHADER_MANAGER_CHANGE_UPDATE_RATE);
			//register_event(m_update_rate);
		}

		void ShaderManager::change_update_rate(const unsigned long update_rate)
		{
			if (update_rate != m_update_rate)
			{
				if (update_rate >= UPDATE  &&  update_rate <= UPDATE_SLOW)
				{
					unregister_event(m_update_rate);
					m_update_rate = update_rate;
					register_event(m_update_rate);
				}
			}
		}

	} /* namespace Core */
} /* namespace TEXEngine */