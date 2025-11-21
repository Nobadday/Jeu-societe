#pragma once
#ifndef _INC_AUDIO_ENGINE_HPP
#define _INC_AUDIO_ENGINE_HPP

#include "../Common.hpp"
#include "AssetManager.hpp"


class AudioEngine
{
public:

	AudioEngine(void);
	void SetAssetManager(AssetManager* _assetManager);
	~AudioEngine(void);

	//Play sound by his name given
	//PROTECTION : You cant play sound who isnt in asset manager (logic)
	void PlaySound(const std::string& _soundName, bool _loop = false);
	//Parse the vector of sound, and pop stopped sound
	void CleanOldSound(void);
	//Togle pause or play on the first specified sound
	void TogglePauseSound(const std::string& _soundName);
	//Set the volume for all sound
	void SetSoundVolume(float& _vol);

	//Play music by his name given
	//PROTECTION : You cant play the same music playing (if he stopped/paused you can)
	//PROTECTION : You cant play music who isnt in asset manager (logic)
	void PlayMusic(const std::string& _musicName, bool _loop = false);
	void SetMusicVolume(float& _vol);
	void TogglePauseMusic(void);
	void StopMusic(void);


private:
	std::vector< sf::Sound> m_soundVec;
	AssetManager* m_assetManager;

	sf::Music* m_music;
	std::string m_currentMusic;
	float m_musicVol;
	float m_soundVol;

};

#endif // _INC_AUDIO_ENGINE_HPP