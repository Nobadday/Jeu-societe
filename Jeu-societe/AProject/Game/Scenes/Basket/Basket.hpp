#pragma once
#ifndef _INC_BASKET_HPP
#define _INC_BASKET_HPP

#include "../../../Common.hpp"
#include "../../../Scenes/Scene.hpp"

class Basket : public SceneBase
{
private:
	//enum GameState
	//{
	//	STATE_PLAYING,
	//	STATE_GAMEOVER
	//};

	//struct BasketPlayer
	//{
	//	short m_id = 0;
	//	bool m_won = 0;
	//	int m_winOrder = 0;  // NOUVEAU: Ordre de victoire (1 = premier, 2 = deuxi�me, etc.)
	//	
	//	sf::Sprite m_ballSprite;
	//	sf::Sprite m_hoopSprite;

	//	sf::CircleShape m_ballShape;
	//	sf::VertexArray m_aimLine;
	//};

	//struct SceneData
	//{
	//	sf::Text timerText;
	//	sf::Text winnerText;
	//	
	//	std::vector<BasketPlayer> players;
	//	
	//	GameData* gameData = NULL;
	//	
	//	GameState state = STATE_PLAYING;
	//	float timer;
	//	int winnerCount = 0;  // NOUVEAU: Compteur de gagnants
	//};

	//SceneData* m_data;

public:
	virtual void Load(void);
	virtual void Unload(void);

	virtual void PollEvent(sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderWindow& _renderWindow);

private:
	//void ResetGame(void);
};

#endif // !_INC_BASKET_HPP

