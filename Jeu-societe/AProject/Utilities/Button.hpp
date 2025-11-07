#ifndef _INC_BUTTON_HPP
#define _INC_BUTTON_HPP

#include <SFML/Graphics.hpp>
#include "../Animation/Graphics/SpriteAnimated.hpp"

class Button : public SpriteAnimated
{

public:
	Button(void);
	~Button();

	void ButtonSetTexture(TextureAnimated& _texture);
	void ButtonUpdateTexture();
	void ButtonUpdateHitbox();
private:

	sf::FloatRect m_hitBox;


	// Button can be clicked with a key (-1 to turn off)
	sf::Keyboard::Key checkKey;
	// The intended key was pressed
	bool stateKeyHeld;
	// the mouse is ON the button
	bool stateIsON;
	// has been held : wait for mouse button/key release
	bool stateIsBeingHeld;
	// Click confirmed : waiting for checker to retreive
	bool hasBeenClicked;

	bool instantClick;


};

#endif // _INC_BUTTON_HPP