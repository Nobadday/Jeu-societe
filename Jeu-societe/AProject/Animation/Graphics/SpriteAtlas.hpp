#pragma once
#ifndef _INC_SPRITEATLAS_HPP
#define _INC_SPRITEATLAS_HPP

#include "./TextureAtlas.hpp"


// SpriteAtlas, simplified texture atlas use
class SpriteAtlas : public sf::Sprite
{
	private:
		TextureAtlas* m_textureAtlas;

	public:
		SpriteAtlas(void);
		SpriteAtlas(const SpriteAtlas& _copy);

		// If resetRect is set to true, resets the texture rect
		void setTexture(TextureAtlas& _textureAtlas, bool _resetRect = true);

		// Set the Texture and sets the frame
		void SetTexture(TextureAtlas& _textureAtlas, const std::string& _name);

		// Sets the texture rect based on the frame name
		void SetTextureFrame(const std::string& _name);
};


#endif
// Sprite Atlas SFML 2.6.2 || v1.0