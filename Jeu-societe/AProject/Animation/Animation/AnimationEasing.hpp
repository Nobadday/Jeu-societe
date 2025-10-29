#pragma once
#ifndef _INC_ANIMATION_EASING
#define _INC_ANIMATION_EASING

#include "../Common.hpp"
#include "../Utilities/Math.hpp"
#include <string>

namespace ANIMATION_NAMESPACE
{
	class Easing
	{
		public:
			enum Type
			{
				LINEAR,

				INSINE,
				OUTSINE,
				INOUTSINE,

				INQUINT,

				OUTBACK,
				INOUTBACK,

				OUTEXPO,
				INOUTEXPO,

				OUTELASTIC,
				INOUTELASTIC,

				OUTBOUNCE,

				// Easing counter
				EASINGS
			};

			static float GetCoefficient(float _valueX, Easing::Type _easing = Type::LINEAR);
			static const char*& GetName(Easing::Type _easing);
	};

}
#endif

// Animation Easing C++ v1.0