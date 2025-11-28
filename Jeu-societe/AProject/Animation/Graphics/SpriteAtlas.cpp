#include "SpriteAtlas.hpp"



SpriteAtlas::SpriteAtlas(void) : sf::Sprite(),
m_textureAtlas (NULL),
m_originCoef   (0,0)
{

}
SpriteAtlas::SpriteAtlas(const SpriteAtlas& _copy) : sf::Sprite(_copy),
m_textureAtlas (_copy.m_textureAtlas),
m_originCoef   (_copy.m_originCoef)
{

}


void SpriteAtlas::setTexture(TextureAtlas& _textureAtlas, bool _resetRect)
{
	this->m_textureAtlas = &_textureAtlas;
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
		this->UpdateOrigin();
	}
}

void SpriteAtlas::setOrigin(float _x, float _y)
{
	this->m_originCoef.x = _x;
	this->m_originCoef.y = _y;
	this->UpdateOrigin();
}
void SpriteAtlas::setOrigin(const sf::Vector2f& _origin)
{
	this->setOrigin(_origin.x, _origin.y);
}


void SpriteAtlas::UpdateOrigin(void)
{
	const sf::IntRect& rect = this->getTextureRect();
	this->sf::Sprite::setOrigin(this->m_originCoef.x * rect.width, this->m_originCoef.y * rect.height);
}


// Sprite Atlas SFML 2.6.2 || v1.0.3
