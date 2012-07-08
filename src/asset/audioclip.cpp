#include "audioclip.hpp"

namespace TEXEngine {
	namespace Asset {

AudioClip::AudioClip(TEXENG_AUDIO_TYPE inType) 
	: id(-1), type(inType)
{}

AudioClip::AudioClip()
	: id(-1), type(TEXENG_AUDIO_FX)
{}
 
	} /* namespace Asset */
} /* namespace TEXEngine */