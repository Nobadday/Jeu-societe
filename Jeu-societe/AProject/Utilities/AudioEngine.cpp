#include "AudioEngine.hpp"

AudioEngine::AudioEngine(void)
	: m_currentMusic(""), m_assetManager(NULL), m_music(NULL),
	m_musicVol(100), m_soundVol(100)
{


}
void AudioEngine::SetAssetManager(AssetManager* _assetManager)
{
	m_assetManager = _assetManager;
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
		this->m_soundVec.back().play();
		this->m_soundVec.back().setVolume(this->m_soundVol);
		this->m_soundVec.back().setLoop(_loop);
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
	for (auto sound = this->m_soundVec.begin(); sound < this->m_soundVec.end(); ++sound)
	{
		if ((*sound).getStatus() == sf::Sound::Status::Stopped)
		{
			*sound = this->m_soundVec.back();
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
void AudioEngine::SetSoundVolume(float& _vol)
{
	//Store sound vol if we need after
	m_soundVol = _vol;

	for (auto sound = this->m_soundVec.begin(); sound < this->m_soundVec.end(); ++sound)
	{
		(*sound).setVolume(_vol);
	}
}

void AudioEngine::PlayMusic(const std::string& _musicName, bool _loop)
{
	//Check if you play the current music
	if (m_currentMusic == _musicName)
	{
		//Check if music is playing
		sf::Music* music = ((sf::Music*)(m_assetManager->GetAsset(_musicName, AssetManager::MUSIC)));
		if (music->getStatus() == sf::Music::Status::Playing)
		{
			std::cout << "WARNING : you want to play music that already playing," << std::endl 
				<< "You want to restart ? If yes, pause / stop and play it" << std::endl;
		}
		else
		{
			//Launch music
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
			music->play();
			music->setLoop(_loop);
			music->setVolume(this->m_musicVol);
			this->m_music = music;
			m_currentMusic = _musicName;
		}
		else if (music != NULL)
		{
			//Launch music
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
void AudioEngine::SetMusicVolume(float& _vol)
{
	//Store music vol if we need after
	m_musicVol = _vol;
	if (m_currentMusic != "")
	{
		m_music->setVolume(_vol);
	}
}
void AudioEngine::TogglePauseMusic(void)
{
	if (m_currentMusic != "")
	{
		if (this->m_music->getStatus() == sf::Music::Status::Playing)
		{
			this->m_music->pause();
		}
		else
		{
			this->m_music->play();
		}
	}
}
void AudioEngine::StopMusic(void)
{
	this->m_music->stop();
}


