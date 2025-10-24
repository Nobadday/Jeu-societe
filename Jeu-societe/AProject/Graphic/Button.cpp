#include "Button.hpp"

Button::Button(void):SpriteAnimated()
{
	
}

bool Button::isMouseOn(sf::Vector2f _mousePos)
{
	sf::FloatRect bounds = SpriteAnimated::getGlobalBounds();
	if (bounds.contains(_mousePos))
	{
		return true;
	}
	return false;
}
