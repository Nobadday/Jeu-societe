#pragma once
#ifndef _INC_RAND_CARD_HPP
#define _INC_RAND_CARD_HPP

#include "../../../Scenes/Scene.hpp"
#include "../../../Common.hpp"
#include "../../../Ui/Button.hpp"
#include "../../../Ui/Transition/Transition.hpp"
#include "../../../Utilities/AudioEngine/AudioEngine.hpp"



class RandCard : public SceneBase
{
	private:

		//Final ;)
		struct Player
		{
			short id;
		};
		enum GameState
		{
			WAITING_BETWEEN_PLAYER,
			CHOOSE_CARD,
			//Animation 
			ANIMATION,
			END,
			TRANSITION
		};
		enum CardType
		{
			NORMAL,
			BOMB
		};
		struct SceneData
		{
			SpriteAnimated cardChosenSprAnim;
			SpriteAnimated cardUnchosenSprAnim;
			SpriteAnimated iconsChara;
			sf::Sprite background;

			std::vector<CardType> cards;
			std::vector<Player> players;
			std::vector<Player> deadPlayers;
			GameData* gameData; 

			GameState gameState = TRANSITION;
			int cardChosen = 0;
			//Delay when you change card with joystick
			float inputDelay = 0.2f;

			int currentPlayer = 0;
			TextPlus text;

			Timer timer;

			int playerCountLeft = 0;

			AudioEngine* audio;
			TransitionClass transition;
		};
		SceneData* m_data;
		void PrintCards(sf::RenderWindow& _renderWindow);
		void PrintIcons(sf::RenderWindow& _renderWindow);
		void SetCardChosen(int _card);
		void AddCardChosen(int _value);

		void NextPlayer(void);
	public:
		virtual void Load(void);
		virtual void Unload(void);


		virtual void PollEvent(sf::Event& _event);
		virtual void Update(float _deltaTime);
		virtual void Draw(sf::RenderWindow& _renderWindow);
};

#endif // _INC_RAND_CARD_HPP


//How it works ? 
//Copy players data by gameData
//With this copy, i delete players when he dies, and add it to deadPlayers
//At the end, delete player have winner, and we can save the order of players die with deadPlayers
