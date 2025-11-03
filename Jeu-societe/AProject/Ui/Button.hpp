#pragma once 
#ifndef _INC_BUTTON_HPP
#define _INC_BUTTON_HPP

#include "../Animation/Graphics/SpriteAnimated.hpp"

class Button : public SpriteAnimated
{
	public :
	
		enum State
		{
			STATE_IDLE = 0,
			STATE_HOVER,
			STATE_PRESSED,

			BUTTONSTATE_COUNT
		};

		Button(void);


		void Update(float _deltaTime);
		void PollEvent(sf::Event& _event);

		bool isMouseOn(sf::Vector2f _mousePos);
		bool isClicked(sf::Vector2f _mousePos);



		void SetState(State _state);

		State GetState(void);

	private: 
		State m_currentState;
};
	
#endif // !_INC_BUTTON_HPP