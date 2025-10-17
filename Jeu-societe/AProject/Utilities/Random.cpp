#include "Random.hpp"

#define RAND_MAX_FLOAT (float)RAND_MAX

namespace RANDOM_HPP_NAMESPACE
{

void SetSeed(unsigned int _seed)
{
	srand(_seed);
}

void SetSeedPID(void)
{
	SetSeed((unsigned)_getpid());
}

int RandomInt(int _min, int _max)
{
	if (_min > _max)
	{
		//return RandomInt(_max, _min);
		return _max + rand() % (_min - _max + 1);
	}
	return _min + rand() % (_max - _min + 1);
}

float RandomFloat(float _min, float _max)
{
	if (_min > _max)
	{
		return _max + (((float)rand() / RAND_MAX_FLOAT) * (_min - _max));
	}
	return _min + (((float)rand() / RAND_MAX_FLOAT) * (_max - _min));
}

bool Chance(float _value, float _minimumChance, float _maximumChange)
{
	return RandomFloat(_minimumChance, _maximumChange) <= _value;
}

}
// Random C++ v1.0