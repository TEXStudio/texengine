#ifndef TEXENGINE_AUDIOMANAGER_HPP_INCLUDED
#define TEXENGINE_AUDIOMANAGER_HPP_INCLUDED

#if defined(_WIN32)

#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstdio>
#include <map>

#include <fmod.hpp>
#include <fmod.h>

#include "../asset/audioClip.hpp"
#include "../message/TEXEngineInterface.hpp"

using namespace FMOD;

using TEXEngine::Asset::TEXENG_AUDIO_TYPE;
using TEXEngine::Asset::AudioClip;
using TEXEngine::Asset::Listener;

namespace TEXEngine {
	namespace Core {		
		// LISTENER - TO BE RELOCATED
		/////////////////////////////
		class FMODObject
		{
			friend class AudioManager;
			private:
				int m_id;
				FMOD_VECTOR m_position, m_velocity, m_forward, m_up;
			
			public:
				// RELOCATE TO PROTECTED
				FMODObject();
				FMODObject(int id);
				
				void Set(Vector3f &inPos, Vector3f &inVel, Vector3f &inForward, Vector3f &inUp);
				void Set(Listener &in);
				
				int id();
				FMOD_VECTOR & position();
				FMOD_VECTOR & velocity();
				FMOD_VECTOR & direction();
				FMOD_VECTOR & up();
		};

		//////////////////////////////

		class AudioManager : public Object
		{
			private:
				static AudioManager m_audio_manager;

				AudioManager();
				~AudioManager();

				FMOD_RESULT result;
				FMOD_SPEAKERMODE speakermode;
				FMOD_CAPS caps;
				FMOD::System *system;
				FMOD::Channel *ClipChannel;
				FMOD::Channel *MusicChannel;
				FMOD::Sound *sound;
				FMOD::Sound *music;
				unsigned int version;
				int numdrivers;
				bool _init, _deinit;

				float ClipVolume, MusicVolume, MusicVolumeLimit;
				bool MusicPaused, FadeOut, FadeIn, SurroundEnabled;
				std::string CurrentMusic;
				unsigned long m_update_rate;

				std::map<int, FMOD::Sound*> _soundmap; 
				void play_fx(AudioClip *inClip);
				void play_music(AudioClip *inMusic);
				FMOD_VECTOR convert_vector(Vector3f &in);
				void change_update_rate(unsigned long event_code);

				int init(); 
				void update();
				void deinit();

		public: 
				// Returns a handle to the audio manager.
				static AudioManager &get();


				void play_fx_file(const char *filename);
				void play_music_file(const char *filename);
				void play_audio(AudioClip *inClip);
				void set_volume(float value, TEXENG_AUDIO_TYPE Type);
				void pause_music();
				void stop_music();
				void start_music();
				void fade_out();
				void fade_in();
				void update_listener(Listener &in);
				void number_of_listeners(int i);
				void toggle_3D(bool in);
			
				void on_event( const unsigned long event_code , const Message& parameters );
				void setup();
		};
	} //  Core Namespace
} //  TEXEngine Namespace

#endif /* _WIN32 */

#endif // ifndef TEXENGINE_AUDIOMANAGER_HPP_INCLUDED 