#pragma once

#ifndef _INC_ANIMATION_MATH_HPP
#define _INC_ANIMATION_MATH_HPP

#include "../Common.hpp"
// C Libs
#define _USE_MATH_DEFINES
#include <math.h>

namespace ANIMATION_NAMESPACE
{
	class AniMath
	{
		public:
			// Converts FPS to the duration of one frame in seconds
			static float FPSToFrameTime(float _FPS);

			// Converts the duration of one frame in seconds to FPS
			static float FrameTimeToFPS(float _seconds);

			static float MilisecondsToFPS(float _miliseconds);


			// Converts seconds to frametime
			// 5 seconds at 24FPS = 120 seconds
			static float SecondsToFrameTime(float _seconds, float _framerate);

			// Modulo always positive
			// -15 % 60  =  45
			static float ModuloPositiveF(float _value, float _modulo);
			// Modulo always positive
			static int ModuloPositiveI(int _value, int _modulo);

			// Modulo always positive
			// -15 % 60  =  45
			template <typename T>
			static T ModuloPositive(T _value, T _modulo);

			inline static float Interpolate(float _start, float _end, float _coefficient);
	};
}

#endif