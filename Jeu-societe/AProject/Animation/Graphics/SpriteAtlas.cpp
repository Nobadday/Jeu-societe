#include "SpriteAtlas.hpp"



SpriteAtlas::SpriteAtlas(void) : sf::Sprite(),
m_textureAtlas (NULL)
{

}
SpriteAtlas::SpriteAtlas(const SpriteAtlas& _copy) : sf::Sprite(_copy),
m_textureAtlas (_copy.m_textureAtlas)
{

}


void SpriteAtlas::setTexture(TextureAtlas& _textureAtlas, bool _resetRect)
{
	this->sf::Sprite::setTexture(_textureAtlas.GetTextureRef(), _resetRect);
}

void SpriteAtlas::SetTexture(TextureAtlas& _textureAtlas, const std::string& _name)
{
	this->setTexture(_textureAtlas, false);
	this->SetTextureFrame(_name);
}

void SpriteAtlas::SetTextureFrame(const std::string& _name)
{
	if (this->m_textureAtlas != NULL)
	{
		int index = this->m_textureAtlas->FindAtlasFrameIndex(_name);
		if (index != -1)
		{
			this->setTextureRect(this->m_textureAtlas->GetAtlasFrame(index).m_textureRect);
		}
		else
		{
			sf::Vector2u size = this->m_textureAtlas->getSize();
			this->setTextureRect(sf::IntRect(0u, 0u, size.x, size.y));
		}
	}
}


// Sprite Atlas SFML 2.6.2 || v1.0
