#pragma once

#include "../../../Common.hpp"
#include "../../../Scenes/Scene.hpp"
#include "../../../Ui/HealthBar/HealthBar.hpp"
#include "../../../Animation/Graphics.hpp"
#include "../../../Animation/Animation/Timer.hpp" 
#include "../../../Animation/Animation.hpp"

 
class ArmWrestling : public SceneBase
{

private:
	enum State
	{
		NONE = -1,

		GAME,
		END,

		STATE_COUNT
	};

public:

	Timer m_timer;
	TextPlus m_timerText;

	sf::Sprite m_background;
	sf::Sprite test;

	SpriteAtlas m_playerArms;
	HealthBar m_bar;

	float m_currentValue;

	GameData* m_gameData = NULL;

	State m_state;

	virtual void Load(void);
	virtual void Unload(void);
	virtual void PollEvent(sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderWindow& _renderWindow);

private:
	void PopTexture(void);
};