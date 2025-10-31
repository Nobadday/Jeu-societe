#pragma once 
#ifndef _INC_BUTTON_HPP
#define _INC_BUTTON_HPP

#include "../Animation/Graphics/SpriteAnimated.hpp"

class Button : public SpriteAnimated
{
	public :
	
		Button(void);

		void Update(float _dt);
		void Draw(sf::RenderTarget& target, sf::RenderStates states);

		void UpdateTexture();

		void SetTexure(TextureAnimated _texture);
		void SetPosition(sf::Vector2f _pos);


		void SetOrigin(sf::Vector2f _origin);

		bool isMouseOn(sf::Vector2f _mousePos);
		//bool isSelected();
		bool isClicked(sf::Vector2f _mousePos);


		SpriteAnimated GetSpriteAnimated();
		sf::Sprite getSprite();

		sf::Vector2f getPosition();



	private: 
		sf::Vector2f m_originPercent;
};
	
#endif // !_INC_BUTTON_HPP