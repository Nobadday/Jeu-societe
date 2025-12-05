#pragma once
#ifndef _INC_RUSSIAN_ROULETTE_HPP
#define _INC_RUSSIAN_ROULETTE_HPP

#include "../../../Scenes/Scene.hpp"
#include "../../../Common.hpp"
#include "../../../Animation/Graphics.hpp"
#include "../../../Animation/Animation.hpp"

class RussianRoulette : public SceneBase
{
	private:

		//DEBUG, REMOVE WHEN PLAYER STRUCT FROM GAMEDATA COMME
		struct Player
		{
			short id;
			short skin;
			bool isAlive = true;
		};
		enum GameState
		{
			WAITING_BETWEEN_PLAYER,
			//Wait for input
			WAITING_FOR_INPUT,
			//Animation 
			SPINNING,
			END
		};
		struct SceneData
		{
			SpriteAnimated gunSprAnim;
			SpriteAnimated iconsChara;
			sf::Sprite background;

			GameState gameState = WAITING_FOR_INPUT;
			int bullet = -1;
			int bulletUser = 0;
			bool killed = false;

			//DEBUG
			int currentPlayer = 0;
			std::vector<Player> players;
			std::vector<Player> deadPlayers;
			GameData* gameData;
			int nbOfPlayers = 4;
			int playerKilled = -1;
			sf::Font font;
			TextPlus text;
			Timer timerEnd;
			AudioEngine* audio;


		};
		SceneData* m_data;

	public:
		virtual void Load(void);
		virtual void Unload(void);


		virtual void PollEvent(sf::Event& _event);
		virtual void Update(float _deltaTime);
		virtual void Draw(sf::RenderWindow& _renderWindow);
};

#endif // _INC_RUSSIAN_ROULETTE_HPP