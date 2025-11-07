#pragma once 
#ifndef _INC_BUTTON_HPP
#define _INC_BUTTON_HPP

#include "../Animation/Graphics/SpriteAnimated.hpp"

class Button : public SpriteAnimated
{
	public :
	
		Button(void);

		bool isMouseOn(sf::Vector2f _mousePos);
	private: 
};
	
#endif // !_INC_BUTTON_HPP