#include "Button.hpp"

Button::Button(void):SpriteAnimated()
{

}

void Button::Update(float _dt)
{
	
}

void Button::Draw(sf::RenderTarget& target, sf::RenderStates states)
{
	SpriteAnimated::draw(target, states);
}

void Button::UpdateTexture()
{
	sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition();
	
	if (!isMouseOn(mousePos) && !isClicked(mousePos))
	{
		SpriteAnimated::SetAnimation("IDLE", true);
	}
	else if (isMouseOn(mousePos) && !isClicked(mousePos))
	{
		SpriteAnimated::SetAnimation("ON", true);
	}
	else if (isClicked(mousePos))
	{
		SpriteAnimated::SetAnimation("HELD", true);
	}

}


void Button::SetTexure(TextureAnimated _texture)
{
	SpriteAnimated::SetTexture(_texture);
}

void Button::SetPosition(sf::Vector2f _pos)
{
	SpriteAnimated::setPosition(_pos);
}

void Button::SetOrigin(sf::Vector2f _origin)
{
	this->m_originPercent = _origin;
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


SpriteAnimated Button::GetSpriteAnimated()
{
	return SpriteAnimated();
}

sf::Sprite Button::getSprite()
{
	return SpriteAnimated::Sprite();
}

sf::Vector2f Button::getPosition()
{
	return SpriteAnimated::getPosition();
}

