#include "Button.hpp"

Button::Button():SpriteAnimated()
{
	this->m_currentState = STATE_IDLE;
}

void Button::Update(float _deltaTime)
{
	SpriteAnimated::Update(_deltaTime);



}

void Button::PollEvent(sf::Event& _event)
{
	if (this->isClicked(sf::Vector2f( _event.mouseMove.x, _event.mouseMove.x )))
	{
		this->m_currentState = STATE_PRESSED;
	}
	else if (this->isMouseOn(sf::Vector2f( _event.mouseMove.x, _event.mouseMove.x )))
	{
		this->m_currentState = STATE_HOVER;
	}
	else
	{
		this->m_currentState = STATE_IDLE;
	}

}



bool Button::isMouseOn(sf::Vector2f _mousePos)
{
	sf::FloatRect rect = Sprite::getGlobalBounds();
	if (rect.contains(_mousePos))
	{
		return true;
	}
	return false;
}

bool Button::isClicked(sf::Vector2f _mousePos)
{
	if (this->isMouseOn(_mousePos) && (sf::Mouse::isButtonPressed(sf::Mouse::Left)))
	{
		return true;
	}
	return false;
}



void Button::SetState(Button::State _state)
{
	this->m_currentState = _state;
}


Button::State Button::GetState(void)
{
	return this->m_currentState;
}