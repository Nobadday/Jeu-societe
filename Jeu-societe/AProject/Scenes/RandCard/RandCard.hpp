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

		int currentNbOfCards = -1;	//Change it
		std::vector<CardType> cards;


		GameState gameState = CHOOSE_CARD;


		//DEBUG
		int currentPlayer = 0;
		int nbOfPlayers = 4;
		sf::Font font;
		sf::Text text;

	};
	SceneData* m_data;

public:
	virtual void Load(void);
	virtual void Unload(void);


	virtual void PollEvent(sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderWindow& _renderWindow);
};

#endif // _INC_RAND_CARD_HPP


//Idée : un sprite normal pour afficher les cartes sans animation (sauf déplacement),
// et un sprite animé pour les effets spéciaux (explosion, la retourner, etc)