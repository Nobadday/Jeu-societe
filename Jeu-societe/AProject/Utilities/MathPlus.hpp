#pragma once
#ifndef _INC_MATH_PLUS
#define _INC_MATH_PLUS

#ifndef _INC_MATH
#define _USE_MATH_DEFINES
#include <math.h>
#else
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#include <math.h>
#endif
#endif

// PI/180, degrees to radians
#define MP_DEG_RAD 0.01745329251994329576923690768489
// 180/PI, radians to degrees
#define MP_RAD_DEG 57.295779513082320876798154814105
// PI*2
#define MP_PI_X2 6.28318530717958647692


// Math Plus
namespace mathp
{

	//double DegToRad(double _degrees);
	float DegToRadF(float _degrees);

	//double RadToDeg(double _radians);
	float RadToDegF(float _radians);

	//double Average(double _value, int _valueCount);
	float AverageF(float _value, int _valueCount);

	// Get the angle between two points (in radians)
	float AnglePointsF(float _x1, float _y1, float _x2, float _y2);
	float DistancePointF(float _x1, float _y1, float _x2, float _y2);

	float ModuloF(float _value, float _modulo);

	// Modulo always positive
	// -15 % 60  =  45
	float ModuloPositiveF(float _value, float _modulo);
	int ModuloPositiveI(int _value, int _modulo);

}


#endif
// Math Plus C++ v1.0