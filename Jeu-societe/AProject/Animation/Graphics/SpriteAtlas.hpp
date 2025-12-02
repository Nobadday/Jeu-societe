#pragma once
#ifndef _INC_SPRITEATLAS_HPP
#define _INC_SPRITEATLAS_HPP

#include "./TextureAtlas.hpp"


// SpriteAtlas, simplified texture atlas use
class SpriteAtlas : public sf::Sprite
{
	private:
		TextureAtlas* m_textureAtlas;
		sf::Vector2f m_originCoef;

	public:
		SpriteAtlas(void);
		SpriteAtlas(const SpriteAtlas& _copy);

		// If resetRect is set to true, resets the texture rect
		void setTexture(TextureAtlas& _textureAtlas, bool _resetRect = true);

		// Set the Texture and sets the frame
		void SetTexture(TextureAtlas& _textureAtlas, const std::string& _name);

		// Sets the texture rect based on the frame name
		void SetTextureFrame(const std::string& _name);

		void setOrigin(float _x, float _y);
		void setOrigin(const sf::Vector2f& _origin);

	private:
		void UpdateOrigin(void);
};


#endif
// Sprite Atlas SFML 2.6.2 || v1.0.3