#pragma once
#ifndef _INC_PARTICLE_HPP
#define _INC_PARTICLE_HPP


#include <SFML/Graphics.hpp>
#include "../../Animation/Graphics.hpp"


class ParticleBase : public sf::Drawable
{
	public:
		virtual ~ParticleBase(void);

		virtual void Update(float _deltaTime) = 0;
		virtual bool HasExpired(void) = 0;
};

class ParticleTimed : public ParticleBase, protected Timer
{
	public:
		ParticleTimed(void);
		ParticleTimed(float _seconds);


		virtual void Update(float _deltaTime);
		virtual bool HasExpired(void);
};


class ParticleAnimated : public ParticleBase
{
	protected:
		SpriteAnimated m_sprite;

	public:
		ParticleAnimated(void);
		ParticleAnimated(const TextureAnimated& _texture);




};



#endif
// Particles || v0.0