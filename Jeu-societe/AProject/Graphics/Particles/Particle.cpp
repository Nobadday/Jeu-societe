#include "Particle.hpp"


ParticleBase::~ParticleBase(void)
{

}


ParticleTimed::ParticleTimed(void) : Timer()
{

}
ParticleTimed::ParticleTimed(float _seconds) : Timer(_seconds, 1.0f)
{

}
void ParticleTimed::Update(float _deltaTime)
{
	this->Timer::Update(_deltaTime);
}
bool ParticleTimed::HasExpired(void)
{
	return this->IsFinished();
}


ParticleAnimated::ParticleAnimated(void) : 
m_sprite ()
{

}
ParticleAnimated::ParticleAnimated(TextureAnimated& _texture) :
m_sprite ()
{
	this->m_sprite.setTexture(_texture, true);
}

void ParticleAnimated::Update(float _deltaTime)
{

}
bool ParticleAnimated::HasExpired(void)
{
	return this->m_sprite.IsFinished();
}

// Particles || v0.0