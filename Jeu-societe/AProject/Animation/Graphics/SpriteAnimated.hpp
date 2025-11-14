#pragma once
#ifndef _INC_ANIMATION_SPRITEANIMATED_HPP
#define _INC_ANIMATION_SPRITEANIMATED_HPP


#include <SFML/Graphics.hpp>

#include "../Common.hpp"
#include "../Animation/AnimationHandler.hpp"
#include "TextureAnimated.hpp"
#include "AnimatedObject.hpp"


class SpriteAnimated : public sf::Sprite, public AnimatedObject
{
	private:
		sf::Vector2f m_originCoefficient;
		sf::Transform m_animTransform;

	public:
		// Default constructor
		SpriteAnimated(void);
		// Copy constructor
		SpriteAnimated(const SpriteAnimated& _copy);

		// If keepAnimation true; set's animation to last anim
		void setTexture(TextureAnimated& _texture, bool _keepAnimation = false);

		void setOrigin(const sf::Vector2f& _origin);

		sf::FloatRect getGlobalBounds(void) const;

	protected:
		virtual void OnFrameChange(void);

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};


#endif

// SpriteAnimated v1.2.3