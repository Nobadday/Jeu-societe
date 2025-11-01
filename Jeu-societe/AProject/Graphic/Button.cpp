#include "Button.hpp"

Button::Button(void):SpriteAnimated()
{

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
