#pragma once
#ifndef _INC_RAND_CARD_HPP
#define _INC_RAND_CARD_HPP

#include "../../../Scenes/Scene.hpp"
#include "../../../Common.hpp"
#include "../../../Animation/Graphics/SpriteAnimated.hpp"
#include "../../../Animation/Graphics/TextureAnimated.hpp"
#include "../../../Ui/MenuSystem.hpp"



class RandCard : public SceneBase
{
	private:

		//Final ;)
		struct Player
		{
			std::string name;
			short id;
			short dyingTime;
		};
		enum GameState
		{
			//Wait for input
			CHOOSE_CARD,
			//Animation 
			ANIMATION,
			END
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
			//Delay when you change card with joystick
			float inputDelay = 0.2f;


			//DEBUG
			int currentPlayer = 0;
			sf::Font font;
			sf::Text text;

			int playerCountLeft = 0;

			MenuSystem* menuSystem;
			Button* buttonTest;
			TextureAnimated* textanim;

			//Button testButton;
			//TextureAnimated& testTexture;
		};
		SceneData* m_data;
		void PrintCards(sf::RenderWindow& _renderWindow);
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