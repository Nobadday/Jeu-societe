#ifndef _INC_TRANSITION_HPP
#define _INC_TRANSITION_HPP



#include "../../Video+Audio/HighResVideoPlayer.hpp"
#include <SFML/Graphics.hpp>


class TransitionClass
{
public:
	enum Transition
	{
		FADED_IN,
		FADED_OUT		
	};

	//Constructor, with the next transition to load
	TransitionClass(Transition _transition = FADED_IN);
	~TransitionClass();

	void PlayTransition();
	void SetTransition(Transition _transition);
	void Update(float _dt);
	void Draw(sf::RenderWindow& _window);
	bool IsFinished(void);

private:
	HighResVideoPlayer videoPlayer;
	sf::Shader chromaKeyShader;
	Transition currentTransition;
};







#endif //!_INC_TRANSITION_HPP