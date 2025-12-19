#include "AudioEngine.hpp"
#include <algorithm>

// Init Audio engine, set to null all things
AudioEngine::AudioEngine(void)
	: m_currentMusic(""), m_assetManager(nullptr), m_music(nullptr),
	m_soundProtected(nullptr), m_musicVol(50.f), 
	m_soundVol(50.f), m_transition({nullptr})
{
	// Reserve initial capacity to prevent frequent reallocations
	m_soundVec.reserve(20);
}

void AudioEngine::SetAssetManager(AssetManager& _assetManager)
{
	m_assetManager = &_assetManager;
}

AudioEngine::~AudioEngine(void)
{
	// Stop all sounds before clearing
	for (auto& sound : m_soundVec)
	{
		if (sound.getStatus() != sf::Sound::Status::Stopped)
		{
			sound.stop();
		}
	}
	m_soundVec.clear();

	// Stop music before destruction
	if (m_music != nullptr && m_currentMusic != "")
	{
		m_music->stop();
	}

	// Clean up transition music if active
	if (m_transition.nextMusic != nullptr)
	{
		m_transition.nextMusic->stop();
		m_transition.nextMusic = nullptr;
	}
}

void AudioEngine::PlaySound(const std::string& _soundName, bool _loop)
{
	// Validate asset manager
	if (m_assetManager == nullptr)
	{
		std::cout << "ERROR: AssetManager not set in AudioEngine" << std::endl;
		return;
	}

	// Clean outdated sounds before adding new one
	CleanOldSound();

	// Check if soundbuffer exists
	sf::SoundBuffer* sound = static_cast<sf::SoundBuffer*>(m_assetManager->GetAsset(_soundName, AssetManager::SOUND_BUFFER));
	if (sound == nullptr)
	{
		std::cout << "WARNING: Sound '" << _soundName << "' not found in AssetManager" << std::endl;
		return;
	}

	// Add new sound
	m_soundVec.emplace_back();
	m_soundVec.back().setBuffer(*sound);
	m_soundVec.back().setVolume(m_soundVol);
	m_soundVec.back().setLoop(_loop);
	m_soundVec.back().play();
}

void AudioEngine::CleanOldSound(void)
{
	// Remove stopped sounds using erase-remove idiom
	m_soundVec.erase(
		std::remove_if(m_soundVec.begin(), m_soundVec.end(),
			[](const sf::Sound& sound) {
				return sound.getStatus() == sf::Sound::Status::Stopped;
			}),
		m_soundVec.end()
	);
}

void AudioEngine::SetSoundVolume(float _vol)
{
	// Clamp volume to valid range [0, 100]
	m_soundVol = std::max(0.f, std::min(100.f, _vol));

	// Apply to all active sounds
	for (auto& sound : m_soundVec)
	{
		sound.setVolume(m_soundVol);
	}
}

void AudioEngine::AddSoundVolume(float _vol)
{
	SetSoundVolume(m_soundVol + _vol);
}

float AudioEngine::GetSoundVolume(void)
{
	return m_soundVol;
}

void AudioEngine::TogglePauseSound(const std::string& _soundName)
{
	if (m_assetManager == nullptr)
	{
		std::cout << "ERROR: AssetManager not set in AudioEngine" << std::endl;
		return;
	}

	sf::SoundBuffer* target = static_cast<sf::SoundBuffer*>(m_assetManager->GetAsset(_soundName, AssetManager::SOUND_BUFFER));
	if (target == nullptr)
	{
		std::cout << "WARNING: Sound '" << _soundName << "' not found in AssetManager" << std::endl;
		return;
	}

	// Toggle pause for the first matching sound
	for (auto& sound : m_soundVec)
	{
		if (sound.getBuffer() == target)
		{
			if (sound.getStatus() == sf::Sound::Status::Playing)
			{
				sound.pause();
			}
			else if (sound.getStatus() == sf::Sound::Status::Paused)
			{
				sound.play();
			}
			break; // Only toggle first matching sound
		}
	}

	// Clean outdated sounds
	CleanOldSound();
}

void AudioEngine::PlayMusic(const std::string& _musicName, bool _loop, bool _startForSavedPos)
{
	if (m_assetManager == nullptr)
	{
		std::cout << "ERROR: AssetManager not set in AudioEngine" << std::endl;
		return;
	}

	// Get music from asset manager
	sf::Music* music = static_cast<sf::Music*>(m_assetManager->GetAsset(_musicName, AssetManager::MUSIC));
	if (music == nullptr)
	{
		std::cout << "WARNING: Music '" << _musicName << "' not found in AssetManager" << std::endl;
		return;
	}

	// Check if trying to play the same music
	if (m_currentMusic == _musicName)
	{
		if (music->getStatus() == sf::Music::Status::Playing)
		{
			std::cout << "WARNING: Music '" << _musicName << "' is already playing" << std::endl;
			return;
		}
	}
	else if (m_currentMusic != "")
	{
		// Stop current music before starting new one
		StopMusic();
	}

	// Set playing offset if requested
	if (_startForSavedPos && musicPos.find(_musicName) != musicPos.end())
	{
		music->setPlayingOffset(musicPos[_musicName]);
	}

	// Start the music
	music->play();
	music->setLoop(_loop);
	music->setVolume(m_musicVol);
	m_music = music;
	m_currentMusic = _musicName;
}

void AudioEngine::PlayMusic(const std::string& _musicName, sf::Music* _music, bool _loop, bool _startForSavedPos)
{
	if (_music == nullptr)
	{
		std::cout << "ERROR: Music pointer is null" << std::endl;
		return;
	}

	// Check if trying to play the same music
	if (m_currentMusic == _musicName)
	{
		if (m_music != nullptr && m_music->getStatus() == sf::Music::Status::Playing)
		{
			std::cout << "WARNING: Music '" << _musicName << "' is already playing" << std::endl;
			return;
		}
	}
	else if (m_currentMusic != "")
	{
		// Stop current music before starting new one
		StopMusic();
	}

	// Set playing offset if requested
	if (_startForSavedPos && musicPos.find(_musicName) != musicPos.end())
	{
		_music->setPlayingOffset(musicPos[_musicName]);
	}

	// Start the music
	_music->play();
	_music->setLoop(_loop);
	_music->setVolume(m_musicVol);
	m_music = _music;
	m_currentMusic = _musicName;
}

void AudioEngine::PlayMusicTransition(const std::string& _musicName, bool _loop, bool _startForSavedPos, float _transitionDuration, TransitionType _type)
{
	if (m_assetManager == nullptr)
	{
		std::cout << "ERROR: AssetManager not set in AudioEngine" << std::endl;
		return;
	}

	// Check if transition is already in progress
	if (m_transition.nextMusic != nullptr)
	{
		std::cout << "WARNING: Transition already in progress, ignoring new transition request" << std::endl;
		return;
	}

	// Get music from asset manager
	sf::Music* music = static_cast<sf::Music*>(m_assetManager->GetAsset(_musicName, AssetManager::MUSIC));
	if (music == nullptr)
	{
		std::cout << "WARNING: Music '" << _musicName << "' not found in AssetManager" << std::endl;
		return;
	}

	// Setup transition
	m_transition.nextMusic = music;
	m_transition.nextMusicName = _musicName;
	m_transition.transitionDuration = std::max(0.1f, _transitionDuration); // Minimum 0.1s
	m_transition.timer = 0.f;
	m_transition.tansitionType = _type;
	m_transition.loop = _loop;

	// Set starting offset if requested
	if (_startForSavedPos && musicPos.find(_musicName) != musicPos.end())
	{
		m_transition.nextMusic->setPlayingOffset(musicPos[_musicName]);
	}

	// Configure transition music
	m_transition.nextMusic->setVolume(0.f);
	m_transition.nextMusic->setLoop(_loop);

	// Start playing immediately for FADED_MIX
	if (_type == FADED_MIX)
	{
		m_transition.nextMusic->play();
	}
}

void AudioEngine::UpdateMusicTransition(float _dt)
{
	// No transition active
	if (m_transition.nextMusic == nullptr)
	{
		return;
	}

	m_transition.timer += _dt;

	// Transition in progress
	if (m_transition.timer < m_transition.transitionDuration)
	{
		const float progress = m_transition.timer / m_transition.transitionDuration;

		switch (m_transition.tansitionType)
		{
		case FADED_MIX:
			// Crossfade: new music fades in, old music fades out
			m_transition.nextMusic->setVolume(progress * m_musicVol);
			if (m_music != nullptr)
			{
				m_music->setVolume((1.f - progress) * m_musicVol);
			}
			break;

		case FADED_ONE_BY_ONE:
			// Sequential fade: first fade out, then fade in
			if (progress < 0.5f)
			{
				// First half: fade out current music
				const float fadeOutProgress = progress * 2.f;
				if (m_music != nullptr)
				{
					m_music->setVolume((1.f - fadeOutProgress) * m_musicVol);
				}
			}
			else
			{
				// Second half: fade in next music
				if (m_transition.nextMusic->getStatus() != sf::Music::Status::Playing)
				{
					m_transition.nextMusic->play();
				}
				const float fadeInProgress = (progress - 0.5f) * 2.f;
				m_transition.nextMusic->setVolume(fadeInProgress * m_musicVol);
			}
			break;
		}
	}
	else
	{
		// Transition complete
		StopMusic();
		m_music = m_transition.nextMusic;
		m_music->setVolume(m_musicVol);
		m_currentMusic = m_transition.nextMusicName;

		// Reset transition
		m_transition.nextMusic = nullptr;
		m_transition.nextMusicName = "";
		m_transition.timer = 0.f;
	}
}

void AudioEngine::SetMusicVolume(float _vol)
{
	// Clamp volume to valid range [0, 100]
	m_musicVol = std::max(0.f, std::min(100.f, _vol));

	if (m_music != nullptr && m_currentMusic != "")
	{
		m_music->setVolume(m_musicVol);
	}
}

void AudioEngine::AddMusicVolume(float _vol)
{
	SetMusicVolume(m_musicVol + _vol);
}

float AudioEngine::GetMusicVolume(void)
{
	return m_musicVol;
}

std::string AudioEngine::GetMusicName(void)
{
	if (m_music != nullptr && m_currentMusic != "" && 
		m_music->getStatus() == sf::Music::Status::Playing)
	{
		return m_currentMusic;
	}
	return "";
}

void AudioEngine::TogglePauseMusic(void)
{
	if (m_music == nullptr || m_currentMusic == "")
	{
		return;
	}

	if (m_music->getStatus() == sf::Music::Status::Playing)
	{
		musicPos[m_currentMusic] = m_music->getPlayingOffset();
		m_music->pause();
	}
	else if (m_music->getStatus() == sf::Music::Status::Paused)
	{
		m_music->play();
	}
}

void AudioEngine::StopMusic(void)
{
	if (m_music == nullptr || m_currentMusic == "")
	{
		return;
	}

	// Save position before stopping
	musicPos[m_currentMusic] = m_music->getPlayingOffset();
	m_music->stop();
}

bool AudioEngine::IsTransitionFinished(void) const
{
	return m_transition.nextMusic == nullptr;
}


