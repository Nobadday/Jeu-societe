#pragma once
#ifndef _INC_RANDOM_HPP
#define _INC_RANDOM_HPP

#define RANDOM_HPP_NAMESPACE random

#include <process.h>
#include <stdlib.h>

namespace RANDOM_HPP_NAMESPACE
{
	void SetSeed(unsigned int _seed);

	//Sets the random seed with the current pid
	void SetSeedPID(void);

	/// <summary>
	/// Returns an integer from _min to _max
	/// </summary>
	/// <param name="_min">Minimum integer</param>
	/// <param name="_max">Maximum integer</param>
	/// <returns>Return a random int between _min and _max (included)</returns>
	int RandomInt(int _min, int _max);

	/// <summary>
	/// Returns an float from _min to _max
	/// </summary>
	/// <param name="_min">Minimum float</param>
	/// <param name="_max">Maximum float</param>
	/// <returns>Return a random float between _min and _max (included)</returns>
	float RandomFloat(float _min, float _max);

	// Chance between _minChance -> _maxChange
	bool Chance(float _value, float _minimumChance = 0.0f, float _maximumChange = 1.0f);
}
#endif
// Random C++ v1.0