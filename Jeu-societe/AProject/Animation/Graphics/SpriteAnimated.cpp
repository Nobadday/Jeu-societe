#include "SpriteAnimated.hpp"

SpriteAnimated::SpriteAnimated(void) : sf::Sprite()
{
	this->m_originPercentage = { 0.0f, 0.0f };
}


void SpriteAnimated::UpdateFrame(void)
{
	if (this->m_animationSelected != -1)
	{
		AnimationProperties* anim = this->AnimatedObject::m_texture->GetAnimation(this->m_animationSelected);
		AtlasFrame* frame = this->AnimatedObject::m_texture->GetFrame(anim->m_atlasIndex, this->m_animationHandler.GetCurrentFrameInt());
		
		this->setTextureRect(frame->m_textureRect);
		sf::Vector2f scale = this->getScale();

		float a = 1.0f;
		float b = 0.0f;
		float c = 0.0f;
		float d = 1.0f;
		float x = -((frame->m_frameOffset.left + anim->m_offset.x)) * scale.x;
		float y = -((frame->m_frameOffset.top + anim->m_offset.y) * (1.0f - this->m_originPercentage.y)) * scale.y;

		//std::cout << "x : " << 1.0f-(2.0f*this->m_originPercentage.x) << " ";
		//std::cout << "y : " << (1.0f - this->m_originPercentage.y) << "\n";

		if (frame->m_rotated && false)
		{
			sf::Vector2f position = this->getPosition();
			a = 0.0f;
			b = 1.0f;
			c = -1.0f;
			d = 0.0f;


			//x *= 0.0f;
			//y *= -1.0f;
			x -= position.y;
			y += position.x;
			//std::cout << position.x << " , " << position.y << "\n";
		}

		this->m_animTransform = sf::Transform(	a, b, x,
												c, d, y,
												0.0f, 0.0f, 1.0f);
		sf::Vector2f size = this->getGlobalBounds().getSize();
		if (frame->m_flipX)
		{
			
		}
		if (frame->m_rotated)
		{
			//this->m_animTransform.combine(sf::Transform(0.0f, 1.0f, 0.0f,
			//											-1.0f, 0.0f, 0.0f,
			//											a0.0f, 0.0f, 1.0f));
			//this->m_animTransform.rotate(-90.0f, size.x/2.0f, size.y/2.0f);
			//this->m_animTransform.translate(frame->m_frameOffset.width, frame->m_frameOffset.height).rotate(-90.0f);
			
			//this->m_animTransform.rotate(-90.0f).translate(-frame->m_frameOffset.width, -frame->m_frameOffset.height);
			//this->m_animTransform *= sf::Transform(	0.0f, 1.0f, 0.0f,
			//										-1.0f, 0.0f, frame->m_frameOffset.width + (frame->m_frameOffset.left + anim->m_offset.x),
			//										0.0f, 0.0f, 1.0f);

			//this->m_animTransform *= sf::Transform(	0.0f, 1.0f, 0.0f,
			//										-1.0f, 0.0f, frame->m_frameOffset.height * 1.0f,
			//										0.0f, 0.0f, 1.0f);

		}
		this->sf::Transformable::setOrigin(frame->m_textureRect.width * this->m_originPercentage.x, frame->m_textureRect.height * this->m_originPercentage.y);
	}
}

void SpriteAnimated::setTexture(TextureAnimated& _texture, bool _keepAnimation)
{
	this->AnimatedObject::SetTexture(_texture, _keepAnimation);
	this->sf::Sprite::setTexture(_texture.GetTextureRef());
}

sf::FloatRect SpriteAnimated::getGlobalBounds(void) const
{
	return this->m_animTransform.transformRect(this->sf::Sprite::getGlobalBounds());
}


void SpriteAnimated::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform.combine(this->m_animTransform);
	target.draw((sf::Sprite)(*this), states);
}
// SpriteAnimated v1.0