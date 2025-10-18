#pragma once
#ifndef _INC_RAND_CARD_HPP
#define _INC_RAND_CARD_HPP

#include "../Scene.hpp"
#include "../../Common.hpp"
#include "../../Animation/Graphics/SpriteAnimated.hpp"
#include "../../Animation/Graphics/TextureAnimated.hpp"

class RandCard : public SceneBase
{
	private:

		//DEBUG, REMOVE WHEN PLAYER STRUCT FROM GAMEDATA COMME
		struct Player
		{
			std::string name;
		};

		enum GameState
		{
			//Wait for input
			CHOOSE_CARD,
			//Animation 
			ANIMATION
		};
		enum CardType
		{
			NORMAL,
			BOMB
		};
		struct SceneData
		{
			sf::Texture backgroundTex;
			sf::Sprite backgroundSpr;

			sf::Texture staticCardTex;
			sf::Sprite staticCardSpr;


			TextureAnimated cardTexAnim;
			SpriteAnimated cardSprAnim;

			std::vector<CardType> cards;
			std::vector<Player> players;


			GameState gameState = CHOOSE_CARD;
			int cardChosen = 0;

			//DEBUG
			int currentPlayer = 0;
			sf::Font font;
			sf::Text text;

		};
		SceneData* m_data;
		void PrintCards(sf::RenderWindow& _renderWindow);

	public:
		virtual void Load(void);
		virtual void Unload(void);


		virtual void PollEvent(sf::Event& _event);
		virtual void Update(float _deltaTime);
		virtual void Draw(sf::RenderWindow& _renderWindow);
};

#endif // _INC_RAND_CARD_HPP