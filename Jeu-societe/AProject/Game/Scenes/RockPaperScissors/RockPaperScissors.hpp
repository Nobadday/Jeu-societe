#pragma once
#ifndef _INC_ROCKPAPERSCISSORS_HPP
#define _INC_ROCKPAPERSCISSORS_HPP


#include "../../Common.hpp"
#include "../../Scenes/Scene.hpp"
#include "../../Animation/Animation/Timer.hpp" 
#include "../../Animation/Graphics.hpp"
#include "../../Animation/Animation.hpp"


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
		STATE_NONE,

		STATE_WARMUP,
		STATE_PLAY,
		STATE_PAUSE,
		STATE_ENDGAME,
	};

	struct SceneData
	{
		sf::Sprite spriteTab[6];                                                                                                                             
		sf::Text timerText;

		SpriteAnimated playerChoiceSprite[2];
		Timer timer;


		sf::Text victoryText;
		State state = STATE_NONE;
		GameData* gameData = NULL;

		RPS_Choice playersChoice[4];
		int roundNB = 0;

		anim::Animator animator[2];
	};
	SceneData* m_data;

public:
	virtual void Load(void);
	virtual void Unload(void);

	virtual void PollEvent(sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderWindow& _renderWindow);

private:
	void UpdatePlayerChoiceTexture(void);
};


#endif // !_INC_ROCKPAPERSCISSORS_HPP