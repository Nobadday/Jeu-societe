#pragma once
#ifndef _INC_ROCKPAPERSCISSORS_HPP
#define _INC_ROCKPAPERSCISSORS_HPP

#include "../../Common.hpp"
#include "../../Scenes/Scene.hpp"
#include "../../Animation/Animation/Timer.hpp" 


class RockPaperScissors : public SceneBase
{
	private:
	enum RPS_Choice
	{
		RPS_NONE = -1,
		RPS_ROCK,
		RPS_PAPER,
		RPS_SCISSORS,
		RPS_CHOICE_COUNT
	};

	enum State
	{
		STATE_WARMUP,
		STATE_PLAY,
		STATE_PAUSE,
		STATE_VICTORY
	};

	struct SceneData
	{
		sf::Sprite spriteTab[6];                                                                                                                             
		sf::Texture textureTab[6];

		//sf::Sprite player1ChoiceSprite;
		//sf::Sprite player2ChoiceSprite;
		Timer timer;

		sf::Text timerText;
		sf::Font font;

		sf::Text victoryText;
		State state;
		GameData* gameData;

		RPS_Choice playersChoice[4];
	
	};
	SceneData* m_data;

public:
	virtual void Load(void);
	virtual void Unload(void);

	virtual void PollEvent(sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderWindow& _renderWindow);
};


#endif // !_INC_ROCKPAPERSCISSORS_HPP