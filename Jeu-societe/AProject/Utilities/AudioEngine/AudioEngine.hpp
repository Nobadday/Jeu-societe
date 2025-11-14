#pragma once
#ifndef _INC_AUDIO_ENGINE_HPP
#define _INC_AUDIO_ENGINE_HPP

#include "../../Common.hpp"
#include "../AssetManager.hpp"
#include <SFML/Audio.hpp>
#include <map>

class AudioEngine
{
public:




private:
	std::vector< sf::Sound> m_soundVec;
	AssetManager assetManager;


};





#endif // _INC_AUDIO_ENGINE_HPP