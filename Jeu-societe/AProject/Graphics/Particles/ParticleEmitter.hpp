#pragma once
#ifndef _INC_PARTICLEEMITTER_HPP
#define _INC_PARTICLEEMITTER_HPP


#include "Particle.hpp"
#include <vector>


class ParticleEmitterBase : public sf::Drawable
{
	private:
		std::vector<ParticleBase> m_particles;
		ParticleEmitterBase* m_parent;


	public:
		ParticleEmitterBase(void);
		virtual ~ParticleEmitterBase(void);

		
		virtual void Update(float _deltaTime) = 0;

};








#endif