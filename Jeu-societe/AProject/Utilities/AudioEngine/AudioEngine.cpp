#include "AudioEngine.hpp"

//Init Audio engine, set to null all things
AudioEngine::AudioEngine(void)
	: m_currentMusic(""), m_assetManager(nullptr), m_music(nullptr),
	m_soundProtected(nullptr), m_musicVol(50.f), 
	m_soundVol(50.f), m_transition({nullptr})
{
	//initial size to prevent bug
	this->m_soundVec.resize((size_t)20);

}
void AudioEngine::SetAssetManager(AssetManager& _assetManager)
{
	m_assetManager = &_assetManager;
}
AudioEngine::~AudioEngine(void)
{
	m_soundVec.clear();
	if (m_currentMusic != "")
	{
		m_music->stop();
	}
}

void AudioEngine::PlaySound(const std::string& _soundName, bool _loop)
{
	//Clean outdated sound
	this->CleanOldSound();

	//Check if soundbuffer exist
	sf::SoundBuffer* sound = ((sf::SoundBuffer*)(m_assetManager->GetAsset(_soundName, AssetManager::SOUND_BUFFER)));
	if (sound != NULL)
	{
		//Launch sound
		this->m_soundVec.resize(this->m_soundVec.size() + 1);
		this->m_soundVec.back().setBuffer(*sound);
		this->m_soundVec.back().setVolume(this->m_soundVol);
		this->m_soundVec.back().setLoop(_loop);
		this->m_soundVec.back().play();
	}
	else
	{
		std::cout << "WARNING : sound isnt in asset manager" << std::endl;
	}

	//Debug
	//std::cout << "PlaySound, nb of sound : " << this->m_soundVec.size() << std::endl;
}
void AudioEngine::CleanOldSound(void)
{
	//Debug 
	//std::cout << "Clean old sound, nb of sound : " << this->m_soundVec.size() << std::endl;


	//Remove by swap end pos could have problem if back sound is also stopped
	//Not big problem, but sound not removed, it will be at the next clean
	//for (auto sound = this->m_soundVec.end(); sound > this->m_soundVec.begin(); sound--)
	//{
	//	if ((*sound).getStatus() == sf::Sound::Status::Stopped)
	//	{
	//		*sound = this->m_soundVec.back();
	//		this->m_soundVec.pop_back();
	//	}
	//}



	//std::cout << "Sounds debug :" << std::endl;
	for (int sound = this->m_soundVec.size() - 1; sound >= 0 ; sound--)
	{

		auto& truesound = this->m_soundVec[sound];
		//std::cout << "Sounds n " << sound << " status : " << truesound.getStatus() <<std::endl;



			if (truesound.getStatus() == sf::Sound::Status::Stopped)
			{
				truesound = this->m_soundVec.back();
				this->m_soundVec.pop_back();
			}
	}

	//for (auto& sound : this->m_soundVec)
	//{
	//	if (sound.getStatus() == sf::Sound::Status::Stopped)
	//	{
	//		sound = this->m_soundVec.back();
	//		this->m_soundVec.pop_back();
	//	}
	//}
	//std::cout << "Clean old sound finished, nb of sound : " << this->m_soundVec.size() << std::endl;
}

void AudioEngine::SetSoundVolume(float _vol)
{
	//Store sound vol if we need after
	m_soundVol = _vol;

	for (auto sound = this->m_soundVec.begin(); sound < this->m_soundVec.end(); ++sound)
	{
		(*sound).setVolume(_vol);
	}
}
void AudioEngine::AddSoundVolume(float _vol)
{
	if (m_soundVol + _vol > 100.f)
	{
		SetSoundVolume(100.f);
		m_soundVol = 100.f;
	}
	else if (m_soundVol + _vol < 0.f)
	{
		SetSoundVolume(0.f);
		m_soundVol = 0.f;
	}
	else
	{
		m_soundVol += _vol;
		SetSoundVolume(m_soundVol);
	}
}
float AudioEngine::GetSoundVolume(void)
{
	return m_soundVol;
}
void AudioEngine::TogglePauseSound(const std::string& _soundName)
{
	//if you have more than one sound playing you can toggle
	//only the first
	//Btw this function will be slow if you have a lot of sound
	//(parse all sound and compare name to assetManager

	sf::SoundBuffer* target = ((sf::SoundBuffer*)(m_assetManager->GetAsset(_soundName, AssetManager::SOUND_BUFFER)));

	for (auto& sound : this->m_soundVec)
	{
		if (sound.getBuffer() == target)
		{
			if (sound.getStatus() == sf::Sound::Status::Playing)
			{
				sound.pause();
			}
			else
			{
				sound.pause();
			}
		}
	}
	//Clean outdated sound
	//Dont impacted paused sound, only stopped
	this->CleanOldSound();
}


void AudioEngine::PlayMusic(const std::string& _musicName, bool _loop, bool _startForSavedPos)
{
	//Check if you play the current music
	if (m_currentMusic == _musicName)
	{
		//Check if music is playing
		sf::Music* music = ((sf::Music*)(m_assetManager->GetAsset(_musicName, AssetManager::MUSIC)));
		if (music->getStatus() == sf::Music::Status::Playing)
		{
			std::cout << "WARNING : you want to play the music that already playing," << std::endl 
				<< "You want to restart ? If yes, pause / stop and play it" << std::endl;
		}
		else
		{
			//Launch music
			if (_startForSavedPos)
			{
				music->setPlayingOffset(musicPos[_musicName]);
			}
			music->play();
			music->setLoop(_loop);
			music->setVolume(this->m_musicVol);
			this->m_music = music;
			m_currentMusic = _musicName;
		}
	}
	else
	{
		//Search if asset exist
		sf::Music* music = ((sf::Music*)(m_assetManager->GetAsset(_musicName, AssetManager::MUSIC)));
		if (music != NULL && m_currentMusic != "")
		{
			this->StopMusic();

			//Launch music
			if (_startForSavedPos)
			{
				music->setPlayingOffset(musicPos[_musicName]);
			}
			music->play();
			music->setLoop(_loop);
			music->setVolume(this->m_musicVol);
			this->m_music = music;
			m_currentMusic = _musicName;
		}
		else if (music != NULL)
		{
			//Launch music
			if (_startForSavedPos)
			{
				music->setPlayingOffset(musicPos[_musicName]);
			}
			music->play();
			music->setLoop(_loop);
			this->m_music = music;
			m_currentMusic = _musicName;
		}
		else
		{
			std::cout << "WARNING : your music isnt finded in asset manager" << std::endl;
		}
	}
}
void AudioEngine::PlayMusic(const std::string& _musicName, sf::Music* _music, bool _loop, bool _startForSavedPos)
{
	//Check if you play the current music
	if (m_currentMusic == _musicName)
	{
		//Check if music is playing
		if (m_music->getStatus() == sf::Music::Status::Playing)
		{
			std::cout << "WARNING : you want to play music that already playing," << std::endl
				<< "You want to restart ? If yes, pause / stop and play it" << std::endl;
		}
		else
		{
			//Launch music (the old music stored is the same as the new one)
			if (_startForSavedPos)
			{
				m_music->setPlayingOffset(musicPos[_musicName]);
			}
			m_music->play();
			m_music->setLoop(_loop);
			m_music->setVolume(this->m_musicVol);
		}
	}
	else
	{
		if (_music != NULL && m_currentMusic != "")
		{
			this->StopMusic();

			//Launch music
			if (_startForSavedPos)
			{
				m_music->setPlayingOffset(musicPos[_musicName]);
			}
			_music->play();
			_music->setLoop(_loop);
			_music->setVolume(this->m_musicVol);
			this->m_music = _music;
			m_currentMusic = _musicName;
		}
		else if (_music != NULL)
		{
			//Launch music
			if (_startForSavedPos)
			{
				m_music->setPlayingOffset(musicPos[_musicName]);
			}
			_music->play();
			_music->setLoop(_loop);
			this->m_music = _music;
			m_currentMusic = _musicName;
		}
		else
		{
			std::cout << "WARNING : your music isnt is NULL" << std::endl;
		}
	}

}
void AudioEngine::PlayMusicTransition(const std::string& _musicName, bool _loop, bool _startForSavedPos, float _transitionDuration, TransitionType _type)
{
	//Remove big problem
	//If next music != nullptr ->
	//One transition is in activity : 
	//WE DO NOT TO INTERFER WITH CURRENT TRANSITION
	if (m_transition.nextMusic != nullptr)
	{
		std::cout << "CRITICAL PROBLEM : OMG YOU WANT ADD MUSIC AND TRANSITION BUT CURRENT TRANSITION ISNT FINISHED" << std::endl;
		return;
	}

	//Check if you play the current music
	if (m_currentMusic == _musicName)
	{
		//Check if music is playing
		sf::Music* music = ((sf::Music*)(m_assetManager->GetAsset(_musicName, AssetManager::MUSIC)));
		if (music->getStatus() == sf::Music::Status::Playing)
		{
			std::cout << "WARNING : you want to play with transition the music that already playing," << std::endl
				<< "This is what you want ? Ok so I do it" << std::endl;

			m_transition.nextMusic = music;
			m_transition.transitionDuration = _transitionDuration;
			m_transition.timer = 0.f;
			m_transition.tansitionType = _type;
			m_transition.nextMusic->setVolume(0.f);
			m_transition.nextMusic->setLoop(_loop);
			if (_type == FADED_MIX)
			{
				m_transition.nextMusic->play();
			}
		}
		else
		{
			m_transition.nextMusic = music;
			m_transition.transitionDuration = _transitionDuration;
			m_transition.timer = 0.f;
			m_transition.tansitionType = _type;
			m_transition.nextMusic->setVolume(0.f);
			m_transition.nextMusic->setLoop(_loop);
			if (_type == FADED_MIX)
			{
				m_transition.nextMusic->play();
			}
		}
	}
	else
	{
		//Search if asset exist
		sf::Music* music = ((sf::Music*)(m_assetManager->GetAsset(_musicName, AssetManager::MUSIC)));
		if (music != NULL)
		{
			m_transition.nextMusic = music;
			m_transition.transitionDuration = _transitionDuration;
			m_transition.timer = 0.f;
			m_transition.tansitionType = _type;
			m_transition.nextMusic->setVolume(0.f);
			m_transition.nextMusic->setLoop(_loop);
			if (_type == FADED_MIX)
			{
				m_transition.nextMusic->play();
			}
		}
		else
		{
			std::cout << "WARNING : your music isnt finded in asset manager" << std::endl;
		}
	}
	if (_startForSavedPos)
	{
		m_transition.nextMusic->setPlayingOffset(musicPos[_musicName]);
	}
}
void AudioEngine::UpdateMusicTransition(float _dt)
{
	if (m_transition.nextMusic != nullptr)
	{
		m_transition.timer += _dt;
		if (m_transition.timer < m_transition.transitionDuration)
		{
			switch (m_transition.tansitionType)
			{
				case FADED_MIX:

					m_transition.nextMusic->setVolume(m_transition.timer / m_transition.transitionDuration * m_musicVol);
					m_music->setVolume((m_transition.transitionDuration - m_transition.timer) / m_transition.transitionDuration * m_musicVol);
					break;

				case FADED_ONE_BY_ONE:

					if (m_transition.timer < m_transition.transitionDuration / 2.f)
					{
						m_music->setVolume(((m_transition.transitionDuration / 2.f) - m_transition.timer) / (m_transition.transitionDuration / 2.f) * m_musicVol);
					}
					else
					{
						if (m_transition.nextMusic->getStatus() != sf::Music::Status::Playing)
						{
							m_transition.nextMusic->play();
						}

						//Coeficient du temp, xSon
						//Temp / TempMax * 100
						m_transition.nextMusic->setVolume((m_transition.timer - m_transition.transitionDuration / 2) / (m_transition.transitionDuration / 2) * m_musicVol);
					}
				break;
			}
		}
		else
		{	
			//Stop current music and save his playingOffset
			StopMusic();
			m_music = m_transition.nextMusic;
			m_transition.nextMusic = nullptr;
			m_transition.timer = 0.f;
			m_currentMusic = m_transition.nextMusicName;
			m_transition.nextMusicName = "";
		}
	}
}

void AudioEngine::SetMusicVolume(float _vol)
{
	//Store music vol if we need after
	m_musicVol = _vol;
	if (m_currentMusic != "")
	{
		m_music->setVolume(_vol);
	}
}
void AudioEngine::AddMusicVolume(float _vol)
{
	if (m_musicVol + _vol > 100.f)
	{
		SetMusicVolume(100.f);
		m_musicVol = 100.f;
	}
	else if (m_musicVol + _vol < 0.f)
	{
		SetMusicVolume(0.f);
		m_musicVol = 0.f;
	}
	else
	{
		m_musicVol += _vol;
		SetMusicVolume(m_musicVol);
	}
}
float AudioEngine::GetMusicVolume(void)
{
	return m_musicVol;
}
void AudioEngine::TogglePauseMusic(void)
{
	if (m_currentMusic != "")
	{
		if (m_music->getStatus() == sf::Music::Status::Playing)
		{
			musicPos[m_currentMusic] = m_music->getPlayingOffset();
			m_music->pause();
		}
		else
		{
			m_music->play();
		}
	}
}
void AudioEngine::StopMusic(void)
{
	musicPos[m_currentMusic] = m_music->getPlayingOffset();
	std::cout << "save outset\n";
	m_music->stop();
}


