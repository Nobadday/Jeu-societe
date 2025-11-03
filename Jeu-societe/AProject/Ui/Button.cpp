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
	if (this->IsClicked(sf::Vector2f( (float)_event.mouseMove.x, (float)_event.mouseMove.x )))
	{
		this->m_currentState = STATE_PRESSED;
	}
	else if (this->IsMouseOn(sf::Vector2f((float)_event.mouseMove.x, (float)_event.mouseMove.x )))
	{
		this->m_currentState = STATE_HOVER;
	}
	else
	{
		this->m_currentState = STATE_IDLE;
	}
}


bool Button::IsMouseOn(sf::Vector2f _mousePos)
{
	return this->getGlobalBounds().contains(_mousePos);
}

bool Button::IsClicked(sf::Vector2f _mousePos)
{
	if (this->IsMouseOn(_mousePos) && (sf::Mouse::isButtonPressed(sf::Mouse::Left)))
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