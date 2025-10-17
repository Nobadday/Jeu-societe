#pragma once

#ifndef _INC_SPRITE_ANIMATED_HPP
#define _INC_SPRITE_ANIMATED_HPP

#include <SFML/Graphics.hpp>

#include "../Common.hpp"
#include "../Animation/AnimationHandler.hpp"
#include "TextureAnimated.hpp"
#include "AnimatedObject.hpp"

class SpriteAnimated : public sf::Sprite, public AnimatedObject
{
	private:
		sf::Vector2f m_originPercentage;
		sf::Transform m_animTransform;

	public:
		// Default constructor
		SpriteAnimated(void);

		void UpdateFrame(void);

		// If keepAnimation true; set's animation to last anim
		void setTexture(TextureAnimated& _texture, bool _keepAnimation = false);
		sf::FloatRect getGlobalBounds(void) const;


		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};


#endif
// SpriteAnimated v1.0