#include "Button.hpp"

Button::Button():SpriteAnimated()
{
	this->m_currentState = STATE_IDLE;
	this->m_isClicked = false;
}



void Button::FrameChanged(void)
{
	this->SpriteAnimated::FrameChanged();
}

void Button::PollEvent(sf::Event& _event)
{
	if (!this->m_isClicked)
	{
		switch (_event.type)
		{
		case sf::Event::MouseButtonPressed:
			if (this->GetState() == STATE_ON)
			{
				if (this->IsClicked(_event.mouseMove.x, _event.mouseMove.y))
				{
					std::cout << "clic" << std::endl;
					this->SetState(STATE_PRESSED);
				}
			}
			break;

		case sf::Event::MouseButtonReleased:
			if (this->GetState() == STATE_PRESSED)
			{
				if (_event.mouseButton.button == sf::Mouse::Left && this->IsMouseOn(_event.mouseMove.x, _event.mouseMove.y))
				{
					std::cout << "Button cliqued" << std::endl;
					this->m_isClicked = true;
				}
			}
			break;

		case sf::Event::MouseMoved:

			switch (this->GetState())
			{
				case STATE_IDLE:
					if (this->IsMouseOn(_event.mouseMove.x, _event.mouseMove.y))
					{
						this->SetState(STATE_ON);
					}
					break;
				case STATE_ON:
					if (!this->IsMouseOn(_event.mouseMove.x, _event.mouseMove.y))
					{
						this->SetState(STATE_IDLE);
					}
					break;
				default:
					break;
			}
			break;

		default:
			break;
		}
	}	
}


bool Button::IsMouseOn(const sf::Vector2f& _mousePos)
{
	return this->getGlobalBounds().contains(_mousePos);
}
bool Button::IsMouseOn(int _mouseX, int _mouseY)
{
	return this->getGlobalBounds().contains((float)_mouseX, (float)_mouseY);
}

bool Button::IsClicked(const sf::Vector2f& _mousePos)
{
	return (this->IsMouseOn(_mousePos) && sf::Mouse::isButtonPressed(sf::Mouse::Left));
}
bool Button::IsClicked(int _mouseX, int _mouseY)
{
	return (this->IsMouseOn(_mouseX, _mouseY) && sf::Mouse::isButtonPressed(sf::Mouse::Left));
}


bool Button::HasBeenClicked(void)
{
	if (this->m_isClicked)
	{
		this->m_isClicked = false;
		this->SetState(STATE_IDLE);
		return true;
	}
	return false;
}


void Button::SetState(Button::State _state)
{
	this->m_currentState = _state;
	switch (_state)
	{
	case Button::STATE_IDLE:
		this->SetAnimation("IDLE");
		break;
	case Button::STATE_ON:
		this->SetAnimation("ON");
		break;
	case Button::STATE_PRESSED:
		this->SetAnimation("HELD");
		break;

	default:
		break;
	}
}

Button::State Button::GetState(void)
{
	return this->m_currentState;
}