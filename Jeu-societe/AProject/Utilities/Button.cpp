#include "Button.hpp"

Button::Button(void)
{
	SpriteAnimated();

	






}

Button::~Button()
{
}

void Button::ButtonSetTexture(TextureAnimated& _texture)
{
	SetTexture(_texture, false);

}


void Button::ButtonUpdateTexture()
{
	if (hasBeenClicked || stateIsBeingHeld || stateKeyHeld)
	{
		
		SetAnimation("HELD", false);
	}
	else if (stateIsON)
	{
		SetAnimation("ON", false);
	}
	else
	{
		SetAnimation("IDLE", false);
	}

	ButtonUpdateHitbox();
}
void Button::ButtonUpdateHitbox()
{
	m_hitBox = getGlobalBounds();
}