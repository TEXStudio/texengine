#ifndef TEXENGINE_AUDIOCLIP_HPP_INCLUDED
#define TEXENGINE_AUDIOCLIP_HPP_INCLUDED

#include <string>
#include "assetEnum.h"
#include "../math/vector.hpp"

using TEXEngine::Math::Vector3f;

namespace TEXEngine {

	namespace Core {
		class AssetManager; /* Forward declaration */
		class AudioManager; /* Forward declaration */
	} /* namespace Core */

	using Core::AudioManager;
	using Core::AssetManager;

	namespace Asset	{

struct Listener
{
	int id; 
	Vector3f position, velocity, direction, up;
};

class AudioClip
{
	friend class AssetManager;
	friend class AudioManager;
	protected:
		AudioClip();
		AudioClip(TEXENG_AUDIO_TYPE inType);

	public:	
		int id;
		TEXENG_AUDIO_TYPE type;
		Vector3f position, velocity;
		std::string filename;
};

	} /* namespace Asset */
} /* namespace TEXEngine */

#endif /* ifndef TEXENGINE_AUDIOCLIP_HPP_INCLUDED */