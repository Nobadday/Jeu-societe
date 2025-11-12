#pragma once 
#ifndef _INC_BUTTON_HPP
#define _INC_BUTTON_HPP

#include "../Animation/Graphics.hpp"
#include "../Animation/Graphics/TextPlus.hpp"

class Button : public SpriteAnimated
{
	public :
	
		enum State
		{
			STATE_IDLE = 0,
			STATE_ON,
			STATE_PRESSED,

			BUTTONSTATE_COUNT
		};

		Button();

		virtual void FrameChanged(void);
		void PollEvent(sf::Event& _event);

		bool IsMouseOn(const sf::Vector2f& _mousePos);
		bool IsMouseOn(int _mouseX, int _mouseY);

		//Check if the button is actually clicked
		bool IsClicked(const sf::Vector2f& _mousePos);
		bool IsClicked(int _mouseX, int _mouseY);


		void SetState(State _state);
		State GetState(void);

		bool HasBeenClicked(void);
		
	private: 
		State m_currentState;
		bool m_isClicked;
};
	

//class ButtonText : public Button
//{
//
//	public :
//		ButtonText();
//
//		virtual void FrameChanged(void);
//		
//	private:
//		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
//
//		TextPlus m_text;
//
//};
// Button V1.0

#endif // !_INC_BUTTON_HPP