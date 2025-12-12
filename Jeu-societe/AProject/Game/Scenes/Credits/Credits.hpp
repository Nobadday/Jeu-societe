#ifndef _INC_CREDITS_HPP
#define _INC_CREDITS_HPP

#include "../../../Scenes/Scene.hpp"
#include "../../../Common.hpp"
#include "../../../Ui/Transition/Transition.hpp"
#include "../../../Utilities/AudioEngine/AudioEngine.hpp"


class Credits : public SceneBase
{
private:

	enum GameState
	{
		CREDITS_TRANSITION_1,
		ANIMATION,
		CREDITS_TRANSITION_2
	};

	struct SceneData
	{
		GameState state = CREDITS_TRANSITION_1;


		HighResVideoPlayer videoPlayer;

		AudioEngine* audio = nullptr;
		TransitionClass transition;
	};
	SceneData* m_data;


public:
	virtual void Load(void);
	virtual void Unload(void);


	virtual void PollEvent(sf::Event& _event);
	virtual void Update(float _deltaTime);
	virtual void Draw(sf::RenderWindow& _renderWindow);
};

#endif //!_INC_CREDITS_HPP