#pragma once 
#ifndef _INC_BUTTON_HPP
#define _INC_BUTTON_HPP


#include "../Animation/Graphics.hpp"
#include "../Animation/Graphics/TextPlus.hpp"


class Button : public SpriteAnimated
{
	public:
		enum State
		{
			// Idle
			STATE_IDLE = 0,
			// Mouse is on the button
			STATE_ON,
			// Being held down
			STATE_PRESSED
		};

	private: 
		State m_currentState;
		bool m_isClicked;

	public:
		Button(void);

		// Automatically checks for events and updates the button accordingly
		bool CheckEvent(const sf::Event& _event);

		
		bool CheckMouseMove(const sf::Vector2i& _mousePos);
		bool CheckMouseMove(int _x, int _y);
		bool CheckMouseClick(int _x, int _y, bool _isReleased);


		void SetState(State _state);
		State GetState(void);

		// Virtually clicks on the button
		void Click(void);

		// Returns true if the button has been clicked and sets the value back to false
		bool HasBeenClicked(bool _keepIntact = false);
		

	private:
		bool IsColliding(int _x, int _y);

};
	


#endif
// Button || V1.1