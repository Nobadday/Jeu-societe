#pragma once
#ifndef ROCKPAPERSIZOR_HPP
#define ROCKPAPERSIZOR_HPP

#include "../../Common.hpp"
#include "../../Scenes/Scene.hpp"
#include "../../Animation/Animation/Timer.hpp"

typedef enum RPS_Choice
{
	RPS_NONE = -1,
	RPS_ROCK,
	RPS_PAPER,
	RPS_SCISSORS,
	RPS_CHOICE_COUNT
} RPS_Choice;

class RockPaperSizor : public SceneBase
{
	private:
	struct SceneData
	{
		sf::Sprite spriteTab[6];
		sf::Texture textureTab[6];

		RPS_Choice player1Choice;
		sf::Sprite player1ChoiceSprite;

		RPS_Choice player2Choice;
		sf::Sprite player2ChoiceSprite;

		Timer timer;

		sf::Text timerText;
		sf::Font font;
		bool isStarted = false;

		sf::Text victoryText;
	};
	SceneData* m_data;

public:
	virtual void Load(void);
	virtual void Unload(void);

	virtual void PollEvent(sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderWindow& _renderWindow);
};

#endif // !ROCKPAPERSIZOR_HPP
