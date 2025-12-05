#include "Math.hpp"


namespace ANIMATION_NAMESPACE
{

float AniMath::FPSToFrameTime(float _FPS)
{
	return 1.0f / _FPS;
}

float AniMath::FrameTimeToFPS(float _seconds)
{
	return 1.0f / _seconds;
}

float AniMath::MilisecondsToFPS(float _miliseconds)
{
	return 1000.0f/ _miliseconds;
}

float AniMath::SecondsToFrameTime(float _seconds, float _framerate)
{
	return _seconds * _framerate;
}

float AniMath::ModuloPositiveF(float _value, float _modulo)
{
	float coefficient = _value / _modulo;
	return (coefficient - std::floorf(coefficient)) * _modulo;
}
int AniMath::ModuloPositiveI(int _value, int _modulo)
{
	return (int)AniMath::ModuloPositiveF((float)_value, (float)_modulo);
}


float AniMath::Interpolate(float _start, float _end, float _coefficient)
{
	return _start + ((_end - _start) * _coefficient);
}
int AniMath::InterpolateI(int _start, int _end, float _coefficient)
{
	return (int)(_start + ((_end - _start) * _coefficient));
}


}

// Animation Math || v1.2