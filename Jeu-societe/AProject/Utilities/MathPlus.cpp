#include "MathPlus.hpp"

namespace mathp
{

	float DegToRadF(float _degrees)
	{
		return _degrees * (float)MP_DEG_RAD;
	}

	float RadToDegF(float _radians)
	{
		return _radians * (float)MP_RAD_DEG;
	}

	float AverageF(float _value, int _valueCount)
	{
		return _value / _valueCount;
	}

	float AnglePointsF(float _x1, float _y1, float _x2, float _y2)
	{
		return atan2f(_y2 - _y1, _x2 - _x1);
	}

	float DistancePointF(float _x1, float _y1, float _x2, float _y2)
	{
		return sqrtf(((_x2 - _x1) * (_x2 - _x1)) + ((_y2 - _y1) * (_y2 - _y1)));
	}

	float ModuloF(float _value, float _modulo)
	{
		return fmodf(_value, _modulo);
	}

	float ModuloPositiveF(float _value, float _modulo)
	{
		float coefficient = _value / _modulo;
		return (coefficient - floorf(coefficient)) * _modulo;
	}

	int ModuloPositiveI(int _value, int _modulo)
	{
		return (int)ModuloPositiveF((float)_value, (float)_modulo);
	}

}

// Math Plus C++ v1.0