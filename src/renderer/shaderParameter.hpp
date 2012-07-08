#ifndef TEXENGINE_SHADERPARAMETER_HPP_INCLUDED
#define TEXENGINE_SHADERPARAMETER_HPP_INCLUDED

#include <string>

#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include "../util/utilenum.h"

using TEXEngine::Util::TEXENG_PARAMETER_TYPE;


using namespace std;

namespace TEXEngine
{
	namespace Core
	{

		//Encapsulate CG parameters in a struct
		struct ShaderParameter
		{
			string m_parameter_name;
			CGparameter m_cg_parameter_handle;
			TEXENG_PARAMETER_TYPE m_eType;
		};
	}
}
#endif /* TEXENGINE_SHADERPARAMETER_HPP_INCLUDED */