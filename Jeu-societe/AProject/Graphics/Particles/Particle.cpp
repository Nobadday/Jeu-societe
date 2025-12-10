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




// Particles || v0.0