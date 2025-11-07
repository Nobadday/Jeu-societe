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
		std::vector<int> m_gonnaPlayIndex;
		std::vector<int> m_WinIndex;

		void InitMiniGamePlayer();

		void SetPlayerPlaying(std::vector<int> _gonnaPlayeIndex);

		void AddPlayerPlaying(int _goneAPlayeIndex);

		void AddPlayerWin(int _winIndex);

};

#endif 