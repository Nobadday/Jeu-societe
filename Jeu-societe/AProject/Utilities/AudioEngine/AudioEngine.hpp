#pragma once
#ifndef _INC_AUDIO_ENGINE_HPP
#define _INC_AUDIO_ENGINE_HPP

#include "../../Common.hpp"
#include "../AssetManager.hpp"
#include "../../Animation/Utilities/Math.hpp"


enum TransitionType
{
	FADED_MIX,
	FADED_ONE_BY_ONE

};


class AudioEngine
{
public:

	AudioEngine(void);
	void SetAssetManager(AssetManager& _assetManager);
	~AudioEngine(void);

	//Play sound by his name given
	void PlaySound(const std::string& _soundName, bool _loop = false);
	//Play sound by soundbuffer* given (when you dont use asset manager)
	void PlaySound(sf::SoundBuffer* _soundbuff, bool _loop = false);
	//Play only one sound. If you call the function but sound playing, he dont let
	//you play the sound util the end of previous sound
	void PlaySoundProtected(sf::SoundBuffer* _soundbuff);
	//Parse the vector of sound, and pop stopped sound
	void CleanOldSound(void);

	//[ONLY WITH ASSET MANAGER]
	//Togle pause or play on the first specified sound
	void TogglePauseSound(const std::string& _soundName);
	//Set the volume for all sound
	void SetSoundVolume(float& _vol);

	//Play music by his name given
	//PROTECTION : You cant play the same music playing (if he stopped/paused you can)
	//PROTECTION : You cant play music who isnt in asset manager (logic)
	void PlayMusic(const std::string& _musicName, bool _loop = false);
	//Play music given, store name of music (function when you dont use asset manager)
	void PlayMusic(const std::string& _musicName, sf::Music* _music, bool _loop = false);
	/// <summary>
	/// Remplace music playing by the given with transition.
	/// If any music playing, just play music.
	/// If current music playing is same than given, transition and restart
	/// </summary>
	/// <param name="_musicName"></param>
	/// <param name="_loop">If true, loop the music you given at his end</param>
	/// <param name="_transitionTime">Transition between current music playing and the new</param>
	void PlayMusic(const std::string& _musicName, bool _loop = false, float _transitionDuration = 5.f, TransitionType _type = FADED_ONE_BY_ONE);
	void UpdateMusicTransition(float _dt);

	void SetMusicVolume(float& _vol);
	void TogglePauseMusic(void);
	void StopMusic(void);


private:
	std::vector<sf::Sound> m_soundVec;
	AssetManager* m_assetManager;

	sf::Music* m_music;
	//Music transition settings
	sf::Music* m_nextMusic;
	std::string m_nextMusicName;
	float m_transitionDuration;
	float timer;
	TransitionType m_tansitionType;
	//

	sf::Sound* m_soundProtected = nullptr;
	std::string m_currentMusic;
	float m_musicVol;
	float m_soundVol;

};

#endif // !_INC_AUDIO_ENGINE_HPP