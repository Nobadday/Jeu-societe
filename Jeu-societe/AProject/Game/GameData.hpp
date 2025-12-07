#ifndef _INC_GAMEDATA_HPP
#define _INC_GAMEDATA_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "PlayerData.hpp"

#include "../Utilities/AssetManager.hpp"

#include "../Utilities/AudioEngine/AudioEngine.hpp"

#include "../Utilities/BetterWindow/BetterWindow.hpp"

class Tiled;

class GameData
{
	private:
		
	public:
		sfMod::RenderWindow* m_renderWindow;
		AssetManager* m_assetManager;
		void* m_audioEngine;


		std::vector<PlayerData> m_playerDataList;
		std::vector<int> m_gonnaPlayIndex;
		std::vector<int> m_winIndex;

		std::string m_nextScene;

		Tiled* m_tile;

	public:
		GameData(void);


		void InitMiniGamePlayer();

		void SetPlayerPlaying(std::vector<int> _gonnaPlayeIndex);

		void AddPlayerPlaying(int _goneAPlayeIndex);

		void AddPlayerWin(int _winIndex);

		// Gets the player's index from joystick ID
		// returns -1 if not found
		int GetPlayerIDFromJoystick(unsigned int _joystickId);

		bool IsPlayerParticipating(int _playerID);

};

#endif 