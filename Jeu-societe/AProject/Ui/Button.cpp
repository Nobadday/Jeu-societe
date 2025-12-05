#include "Button.hpp"

Button::Button(void) : SpriteAnimated(),
m_currentState	(STATE_IDLE),
m_isClicked		(false)
{

}

bool Button::CheckEvent(const sf::Event& _event)
{
	switch (_event.type)
	{
		case sf::Event::MouseButtonPressed:
		case sf::Event::MouseButtonReleased:
			if (_event.mouseButton.button == sf::Mouse::Left)
			{
				return this->CheckMouseClick(_event.mouseButton.x, _event.mouseButton.y, _event.type == sf::Event::MouseButtonReleased);
			}
			break;

		case sf::Event::MouseMoved:
			return this->CheckMouseMove(_event.mouseMove.x, _event.mouseMove.y);
			break;

		default:
			break;

	}
	return false;
}


bool Button::HasBeenClicked(bool _keepIntact)
{
	if (_keepIntact)
	{
		return this->m_isClicked;
	}

	if (this->m_isClicked)
	{
		this->m_isClicked = false;
		this->SetState(STATE_IDLE);
		return true;
	}
	return false;
}

bool Button::IsColliding(int _x, int _y)
{
	return this->getGlobalBounds().contains((int)_x, (int)_y);
}


bool Button::CheckMouseMove(const sf::Vector2i& _mousePos)
{
	return this->CheckMouseMove(_mousePos.x, _mousePos.y);
}
bool Button::CheckMouseMove(int _x, int _y)
{
	if (this->m_currentState != STATE_PRESSED)
	{
		if (this->IsColliding(_x, _y))
		{
			this->SetState(STATE_ON);
			return true;
		}
		else
		{
			this->SetState(STATE_IDLE);
		}
	}
	return false;
}

bool Button::CheckMouseClick(int _x, int _y, bool _isReleased)
{
	if (_isReleased)
	{
		// Confirmation click
		if (this->m_currentState == STATE_PRESSED)
		{
			this->SetState(STATE_IDLE);
			if (this->IsColliding(_x, _y))
			{
				this->m_isClicked = true;
				return true;
			}
			return false;
		}
		// Else : Missclick
	}
	else
	{
		// Initiation click
		if ((this->m_currentState != STATE_PRESSED) && this->IsColliding(_x, _y))
		{
			this->SetState(STATE_PRESSED);
			return true;
		}
	}
	
	return false;
}


void Button::SetState(Button::State _state)
{
	switch (this->m_currentState = _state)
	{
		case Button::STATE_IDLE:
			this->SetAnimation("IDLE", false);
			break;

		case Button::STATE_ON:
			this->SetAnimation("ON", false);
			break;

		case Button::STATE_PRESSED:
			this->SetAnimation("HELD", false);
			break;

		default:
			break;
	}
}

Button::State Button::GetState(void)
{
	return this->m_currentState;
}

void Button::Click(void)
{
	this->SetState(STATE_PRESSED);
	this->m_isClicked = true;
}



// Button || v1.1