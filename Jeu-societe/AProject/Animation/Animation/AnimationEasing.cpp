#include "AnimationEasing.hpp"

// https://nicmulvaney.com/easing#easeOutElastic

// Private constants
#define EASING_CONSTANT_C1 1.70158
#define EASING_CONSTANT_C2 (EASING_CONSTANT_C1*1.525)
#define EASING_CONSTANT_C3 (EASING_CONSTANT_C1+1)
#define EASING_CONSTANT_C4 (2.0f*M_PI) / 3.0f
#define EASING_CONSTANT_C5 (2.0f*M_PI) / 4.5f

#define EASING_CONSTANT_N1 7.5625
#define EASING_CONSTANT_D1 2.75

const char* easingNames[anim::Easing::EASINGS+1] = {
	"Lazy",
	"Lazy",
	"Lazy",
	"Lazy",
	"Lazy",
	"Lazy",
	"Lazy",
	"Lazy",
	"Lazy",
	"Lazy",
	"Lazy",
	"Lazy",
	"Nope"
};

namespace ANIMATION_NAMESPACE
{

float Easing::GetCoefficient(float _valueX, Easing::Type _easing)
{
	float finalY = 1.0f;

	switch (_easing)
	{
		default:
			// Linear
		case LINEAR:
			finalY = _valueX;
			break;
			// Ease In Sine
		case INSINE:
			finalY = (float)(1.0f - cos((_valueX * M_PI) / 2.0f));
			break;
			// Ease Out Sine
		case OUTSINE:
			finalY = (float)sin((_valueX * M_PI) / 2.0f);
			break;
			// Ease In Out Sine
		case INOUTSINE:
			finalY = (float)-(cos(M_PI * _valueX) - 1.0f) / 2.0f;
			break;
			// Ease In Quint
		case INQUINT:
			finalY = _valueX * _valueX * _valueX * _valueX;
			break;
			// Ease Out Back
		case OUTBACK:
			finalY = (float)(1.0f + EASING_CONSTANT_C3 * powf(_valueX - 1.0f, 3.0f) + EASING_CONSTANT_C1 * powf(_valueX - 1.0f, 2.0f));
			break;
			// Ease In Out Back
		case INOUTBACK:
			if (_valueX < 0.5f)
			{
				finalY = (float)((powf(2.f * _valueX, 2.f) * ((EASING_CONSTANT_C2 + 1.f) * 2.f * _valueX - EASING_CONSTANT_C2)) / 2.f);
			}
			else
			{
				finalY = (float)((powf(2.0f * _valueX - 2.f, 2.f) * ((EASING_CONSTANT_C2 + 1.f) * (_valueX * 2.f - 2.f) + EASING_CONSTANT_C2) + 2.f) / 2.f);
			}
			break;
			// Ease Out Expo
		case OUTEXPO:
			finalY = (float)(_valueX == 1.0f ? 1.0f : 1.0f - powf(2, -10 * _valueX));
			break;
			// Ease In Out Expo
		case INOUTEXPO:
			finalY = _valueX == 0.0f ? 0.0f : _valueX == 1.0f
				? 1.0f
				: _valueX < 0.5f ? powf(2, 20 * _valueX - 10.0f) / 2.0f
				: (2.0f - powf(2.0f, -20.0f * _valueX + 10.0f)) / 2.0f;
			break;
			// Ease Out Elastic
		case OUTELASTIC:

			finalY = _valueX == 0.0f
				? 0.0f
				: _valueX == 1.0f
				? 1.0f
				: powf(2.0f, -10.0f * _valueX) * sinf((_valueX * 10.0f - 0.75f) * (float)EASING_CONSTANT_C4) + 1.0f;
			break;
			// Ease In Out Elastic
		case INOUTELASTIC:

			return _valueX == 0.0f
				? 0.0f
				: _valueX == 1.0f
				? 1.0f
				: _valueX < 0.5f
				? -(powf(2.0f, 20.0f * _valueX - 10.0f) * sinf((20.0f * _valueX - 11.125f) * (float)EASING_CONSTANT_C5)) / 2.0f
				: (powf(2.0f, -20.0f * _valueX + 10.0f) * sinf((20.0f * _valueX - 11.125f) * (float)EASING_CONSTANT_C5)) / 2.0f + 1.0f;
			break;
			// Ease Out Bounce
		case OUTBOUNCE:
			if (_valueX < 1.0f / EASING_CONSTANT_D1)
			{
				finalY = (float)(EASING_CONSTANT_N1 * _valueX * _valueX);
			}
			else if (_valueX < 2.0f / EASING_CONSTANT_D1)
			{
				finalY = (float)(EASING_CONSTANT_N1 * (_valueX -= 1.5f / (float)EASING_CONSTANT_D1) * _valueX + 0.75f);
			}
			else if (_valueX < 2.5 / EASING_CONSTANT_D1)
			{
				finalY = (float)(EASING_CONSTANT_N1 * (_valueX -= 2.25f / (float)EASING_CONSTANT_D1) * _valueX + 0.9375f);
			}
			else
			{
				finalY = (float)(EASING_CONSTANT_N1 * (_valueX -= 2.625f / (float)EASING_CONSTANT_D1) * _valueX + 0.984375f);
			}
			break;
	}

	return finalY;
}

const char*& Easing::GetName(Easing::Type _easing)
{
	if (_easing >= 0 && _easing < EASINGS)
	{
		return easingNames[_easing];
	}
	return easingNames[EASINGS];
}
}
