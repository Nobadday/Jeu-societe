#pragma once
#ifndef _INC_PARTICLEEMITTER_HPP
#define _INC_PARTICLEEMITTER_HPP


#include "Particle.hpp"
#include <vector>


class ParticleEmitterBase
{
	private:
		std::vector<ParticleBase> m_particles;
		ParticleEmitterBase* m_parent;
	

	public:
		ParticleEmitterBase(void);



};








#endif