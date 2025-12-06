#pragma once
#ifndef _INC_RANDOM_HPP
#define _INC_RANDOM_HPP

#define RANDOM_HPP_NAMESPACE random
#define RANDOM_MT_HPP_NAMESPACE randmt

#include <process.h>
#include <stdlib.h>
#include <random>


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

	// Returns a random index based on a chance probability vector
	// If all probabilities fails, the last one will have 100% chance
	size_t ChanceList(const std::vector<float>& _chances, float _minimumChance = 0.0f, float _maximumChange = 1.0f);
}


// Random with larger values and more random
namespace RANDOM_MT_HPP_NAMESPACE
{
	void SetSeed(unsigned _seed);
	void SetSeedDefault(void);
	void SetSeedPID(void);

	inline unsigned Rand(void);

	int RandomInt(int _min, int _max);

	float RandomFloat(float _min, float _max);

	bool Chance(float _value, float _minimumChance = 0.0f, float _maximumChange = 1.0f);

	// Returns a random index based on a chance probability vector
	// If all probabilities fails, the last one will have 100% chance
	size_t ChanceList(const std::vector<float>& _chances, float _minimumChance = 0.0f, float _maximumChange = 1.0f);

	unsigned GetSeed(void);
	unsigned GetSeedDefault(void);
}

#endif
// Random C++ v1.2