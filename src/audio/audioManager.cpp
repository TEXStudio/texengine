#include "audioManager.hpp"

#if defined(_WIN32)

using namespace TEXEngine::Asset;
using namespace TEXEngine::Math;

namespace TEXEngine
{
	namespace Core
	{

FMODObject::FMODObject() 
	: m_id (0)
{}

FMODObject::FMODObject(int id)
	: m_id (id) 
{}

void FMODObject::Set(Vector3f &inPos, Vector3f &inVel, Vector3f &inForward, Vector3f &inUp)
{
	m_position.x = (float)inPos.x;
	m_position.y = (float)inPos.y;
	m_position.z = (float)inPos.z;
	m_velocity.x = (float)inVel.x;
	m_velocity.y = (float)inVel.y;
	m_velocity.z = (float)inVel.z;
	m_forward.x = (float)inForward.x;
	m_forward.y = (float)inForward.y;
	m_forward.z = (float)inForward.z;
	m_up.x = (float)inUp.x; 
	m_up.y = (float)inUp.y; 
	m_up.z = (float)inUp.z; 
}

void FMODObject::Set(Listener &in)
{
	m_position.x = (float)in.position.x;
	m_position.y = (float)in.position.y;
	m_position.z = (float)in.position.z;
	m_velocity.x = (float)in.velocity.x;
	m_velocity.y = (float)in.velocity.y;
	m_velocity.z = (float)in.velocity.z;
	m_forward.x = (float)in.direction.x;
	m_forward.y = (float)in.direction.y;
	m_forward.z = (float)in.direction.z;
	m_up.x = (float)in.up.x; 
	m_up.y = (float)in.up.y; 
	m_up.z = (float)in.up.z; 
}

FMOD_VECTOR & FMODObject::position()
{
	return m_position;
}

FMOD_VECTOR & FMODObject::velocity()
{
	return m_velocity;
}

FMOD_VECTOR & FMODObject::direction()
{
	return m_forward;
}

FMOD_VECTOR & FMODObject::up()
{
	return m_up;
}

int FMODObject::id()
{
	return m_id;
}

/* AudioManager */
AudioManager AudioManager::m_audio_manager;

AudioManager::AudioManager() 
	: Object(3) , 
	  ClipVolume (0.5f), MusicVolume (0.5f), MusicVolumeLimit(0.5f),
	  MusicPaused (false), CurrentMusic (""), FadeOut (false),
	  FadeIn (false), SurroundEnabled (false), m_update_rate (UPDATE),
	  _init (false), _deinit (false)
{}

AudioManager::~AudioManager()
{
	deinit(); 
}

AudioManager &AudioManager::get()
{
	return m_audio_manager;
}

int AudioManager::init()
{
	if (!_init)
	{
		char name[256];
		result = FMOD::System_Create(&system);
		result = system->getVersion(&version);
		if (version < FMOD_VERSION)
		{
			printf("Error! You are using an old version of FMOD %08x. This program requires %08x\n", version, FMOD_VERSION);
			return 0;
		}
		else
		{
			result = system->getNumDrivers(&numdrivers);
			if (numdrivers == 0)
			{
				result = system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);

			}
			else
			{
				result = system->getDriverCaps(0, &caps, 0, &speakermode);

				/*
				Set the user selected speaker mode.
				*/
				result = system->setSpeakerMode(speakermode);
				if (caps & FMOD_CAPS_HARDWARE_EMULATED)
				{
					/*
					The user has the 'Acceleration' slider set to off! This is really bad
					for latency! You might want to warn the user about this.
					*/
					result = system->setDSPBufferSize(1024, 10);
				}
				result = system->getDriverInfo(0, name, 256, 0);
				if (strstr(name, "SigmaTel"))
				{
					/*
					Sigmatel sound devices crackle for some reason if the format is PCM 16bit.
					PCM floating point output seems to solve it.
					*/
					result = system->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, FMOD_DSP_RESAMPLER_LINEAR);
				}
			}
			result = system->init(100, FMOD_INIT_NORMAL, 0);
			if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)
			{
				/*
				Ok, the speaker mode selected isn't supported by this soundcard. Switch it
				back to stereo...
				*/
				result = system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
				/*
				... and re-init.
				*/
				result = system->init(100, FMOD_INIT_NORMAL, 0);
			}
			//result = system->setSoftwareChannels(5);

			_init = true; 
			return 1;
		}
	}
	else return 0; 
}

void AudioManager::update() 
{ 
	FMOD_RESULT res = system->update(); 

	if (FadeOut)
	{
		if (MusicVolume > 0.0f)
		{
			MusicVolume -= 0.0001f;
			MusicChannel->setVolume(MusicVolume);
		}
		else
		{
			MusicVolume = 0.0f;
			MusicChannel->setVolume(MusicVolume);
			FadeOut = false; 
		}
	}
	else if (FadeIn)
	{
		if (MusicVolume < MusicVolumeLimit)
		{
			MusicVolume += 0.0001f;
			MusicChannel->setVolume(MusicVolume);
		}
		else
		{
			MusicVolume = MusicVolumeLimit;
			MusicChannel->setVolume(MusicVolume);
			FadeIn = false; 
		}
	}
}

void AudioManager::deinit()
{
	if (!_deinit)
	{
		std::map<int, FMOD::Sound*>::iterator _it;
		for (_it = _soundmap.begin(); _it != _soundmap.end(); _it++) 
			(*_it).second->release();

		system->release();
		_deinit = true; 
	}
}


void AudioManager::play_fx(AudioClip *inClip)
{
	if (inClip != NULL)
	{
	if (inClip->id == -1)
	{
		inClip->id = (int) _soundmap.size(); 
		FMOD::Sound *buff = NULL;
		_soundmap.insert(std::pair<int, FMOD::Sound*>(inClip->id, buff));


		if (SurroundEnabled)
			result = system->createSound(inClip->filename.c_str(), FMOD_3D | FMOD_LOOP_OFF, 0, &_soundmap.rbegin()->second);
		else
			result = system->createSound(inClip->filename.c_str(), FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &_soundmap.rbegin()->second);
		//Sound Pointer Initialisation
		/////////////////////////////////
		//_soundmap.rbegin()->second->setMode(FMOD_LOOP_OFF);

	}
				
	result = system->playSound(FMOD_CHANNEL_FREE, _soundmap.find(inClip->id)->second, false, &ClipChannel);
	ClipChannel->setVolume(ClipVolume);
	if (SurroundEnabled)
	{
		FMODObject Temp;
		Temp.Set(inClip->position, inClip->velocity, inClip->position, inClip->velocity);
		ClipChannel->set3DAttributes(&Temp.position(), &Temp.velocity());
	}
	}
}

void AudioManager::play_music(AudioClip *inMusic)
{
	if (inMusic->id == -1)
	{
		inMusic->id = (int) _soundmap.size(); 
		FMOD::Sound *buff = NULL;
		_soundmap.insert(std::pair<int, FMOD::Sound*>(inMusic->id, buff));
		result = system->createSound(inMusic->filename.c_str(), FMOD_DEFAULT | FMOD_LOOP_NORMAL, 0, &_soundmap.rbegin()->second);

		//Sound Pointer Initialisation
		/////////////////////////////////
		_soundmap.rbegin()->second->setMode(FMOD_LOOP_NORMAL);
	}
			
	result = system->playSound(FMOD_CHANNEL_FREE, _soundmap.find(inMusic->id)->second, false, &MusicChannel);
	MusicChannel->setVolume(MusicVolume);
}

FMOD_VECTOR AudioManager::convert_vector(Vector3f &in)
{
	FMOD_VECTOR Temp;
	Temp.x = (float)in.x;
	Temp.y = (float)in.y;
	Temp.z = (float)in.z;
	return Temp;
}

void AudioManager::play_fx_file(const char *filename)
{
	if (_init)
	{
		if (sound == NULL)
		{
			result = system->createSound(filename, FMOD_DEFAULT, 0, &sound);
			result = system->playSound(FMOD_CHANNEL_FREE, sound, false, &ClipChannel);
			ClipChannel->setVolume(ClipVolume);
		}
		else
		{
			result = system->playSound(FMOD_CHANNEL_FREE, sound, false, &ClipChannel);
		}
	}
}

void AudioManager::play_music_file(const char *filename)
{
	if (_init)
	{
		if ((music == NULL) || (std::string(filename).compare(CurrentMusic) != 0))
		{
			result = system->createStream(filename, FMOD_DEFAULT, 0, &music);
			result = system->playSound(FMOD_CHANNEL_FREE, music, false, &MusicChannel);
			MusicChannel->setVolume(MusicVolume);
			CurrentMusic = filename;
		}
		else
		{
			result = system->playSound(FMOD_CHANNEL_FREE, music, false, &MusicChannel);
		}
	}
}

void AudioManager::play_audio(AudioClip *inClip)
{
	if (_init && inClip)
	{
		if (inClip->type == TEXENG_AUDIO_FX) play_fx(inClip);
		else play_music(inClip);
	}
}

void AudioManager::pause_music()
{	
	MusicPaused = !MusicPaused; 
	result = MusicChannel->setPaused(MusicPaused);
}

void AudioManager::stop_music()
{	
	MusicPaused = true; 
	result = MusicChannel->setPaused(MusicPaused);
}

void AudioManager::start_music()
{	
	MusicPaused = false; 
	result = MusicChannel->setPaused(MusicPaused);
}

void AudioManager::set_volume(float value, TEXENG_AUDIO_TYPE Type)
{
	if (Type == TEXENG_AUDIO_FX)
	{
		ClipVolume = value;
	}
	else
	{
		MusicVolumeLimit = value; 
		MusicVolume = value; 
		MusicChannel->setVolume(MusicVolume);
	}
}

void AudioManager::fade_out()
{
	FadeOut = true;
}

void AudioManager::fade_in()
{
	FadeIn = true;
}

void AudioManager::update_listener(Listener &in)
{
	FMODObject Temp;
	Temp.Set(in.position, in.velocity, in.direction, in.up);
	system->set3DListenerAttributes(Temp.id(), &Temp.position(), &Temp.velocity(), &Temp.direction(), &Temp.up());
}

void AudioManager::number_of_listeners(int i)
{
	result = system->set3DNumListeners(i);
}

void AudioManager::toggle_3D(bool in)
{
	SurroundEnabled = in;
}


// Message-Oriented Functions
/////////////////////////////
void AudioManager::on_event( const unsigned long event_code , const Message& parameters )
{	
	if (event_code == m_update_rate)
		update();
	else if (event_code == INITIALISE)
	{
		int result = init();
	}
	else if (event_code == EXIT)
	{
		deinit();
	}
	else if (event_code == AUDIO_MANAGER_CHANGE_UPDATE_RATE)
	{
		change_update_rate(static_cast<unsigned long>(parameters.get_integral_data(2)));
	}

}

void AudioManager::change_update_rate(unsigned long event_code)
{
	if ( event_code != m_update_rate )
	{
		if ( event_code >= UPDATE  &&  event_code <= UPDATE_SLOW  )
		{
			unregister_event(m_update_rate);
			m_update_rate = (unsigned long)event_code;
			register_event(m_update_rate);
		}
	}
}

void AudioManager::setup()
{
	register_event(INITIALISE);
	register_event(EXIT);
	register_event(AUDIO_MANAGER_CHANGE_UPDATE_RATE);
	register_event(m_update_rate);
	// List Any Other Events that Need to be Listened to

}

	} /* namespace Core */
} /* namespace TEXEngine */

#endif /* _WIN32 */