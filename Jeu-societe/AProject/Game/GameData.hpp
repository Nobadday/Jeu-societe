#ifndef _INC_GAMEDATA_HPP
#define _INC_GAMEDATA_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "PlayerData.hpp"

class GameData
{
	private:
		
	public:
		sf::RenderWindow* m_renderWindow;


		std::vector<PlayerData> m_playerDataList;
		std::vector<int> m_goneAPlayIndex;
		std::vector<int> m_WinIndex;

		void InitMiniGamePlaye(){m_goneAPlayIndex.clear();m_WinIndex.clear();};
		void SetPlayerPlaying(std::vector<int> _goneAPlayeIndex) { m_goneAPlayIndex = _goneAPlayeIndex; };
		void AddPlayerPlaying(int _goneAPlayeIndex) { m_goneAPlayIndex.push_back(_goneAPlayeIndex); };
		void AddPlayerWin(int _winIndex) { m_WinIndex.push_back(_winIndex); };
};

#endif 