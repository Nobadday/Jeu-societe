#include "Animator.hpp"


namespace ANIMATION_NAMESPACE
{

Animator::Animator(void) : Animation(),
m_data			(),
m_usedAnims		(),
m_animEasings	()
{
	this->ResetAnimation();
}

Animator::Animator(float _durationSeconds, float _framerate, bool _loop, float _speed) : Animation(_durationSeconds, _framerate, _loop, _speed),
m_data			(),
m_usedAnims		(),
m_animEasings	()
{
	this->ResetAnimation();
}


bool Animator::AnimateObject(sf::Transformable& _object)
{
	if (this->ShouldUpdate())
	{
		if (this->m_usedAnims[GOTO])
		{
			_object.setPosition(this->GetGoTo());
		}

		
		return true;
	}
	return false;
}
bool Animator::AnimateObject(sf::Sprite& _object)
{
	if (this->ShouldUpdate())
	{
		if (this->m_usedAnims[GOTO])
		{
			_object.setPosition(this->GetGoTo());
		}
		if (this->m_usedAnims[COLOR_TRANSITION])
		{
			_object.setColor(this->GetColor());
		}
		if (this->m_usedAnims[ROTATION])
		{
			_object.setRotation(this->GetRotation());
		}

		return true;
	}
	return false;
}


void Animator::ResetAnimation(void)
{
	for (int i = 0; i < ANIMATION_COUNT; i++)
	{
		m_usedAnims[i] = false;
		m_animEasings[i] = Easing::LINEAR;
	}
}

void Animator::SetAnimationEasing(Animations _animation, Easing::Type _easing)
{
	m_animEasings[_animation] = _easing;
	this->SetShouldUpdate();
}

void Animator::SetGoTo(const sf::Vector2f& _startPos, const sf::Vector2f& _endPos)
{
	this->m_data.goTo[0] = _startPos;
	this->m_data.goTo[1] = _endPos;
	this->m_usedAnims[GOTO] = true;
	this->SetShouldUpdate();
}
void Animator::SetGoTo(const sf::Vector2f& _startOrEnd, bool _start)
{
	this->m_data.goTo[_start] = _startOrEnd;
	this->m_usedAnims[GOTO] = true;
	this->SetShouldUpdate();
}
void Animator::SetGoTo(const sf::Vector2f& _newPosition)
{
	this->SetGoTo(this->m_data.goTo[1], _newPosition);
}
void Animator::SetGoTo(const sf::Transformable& _object, const sf::Vector2f& _newPosition)
{
	this->SetGoTo(_object.getPosition(), _newPosition);
}

void Animator::SetColorTransition(const sf::Color& _startColor, const sf::Color& _endColor)
{
	this->m_data.colorTransition[0] = _startColor;
	this->m_data.colorTransition[1] = _endColor;
	this->m_usedAnims[COLOR_TRANSITION] = true;
	this->SetShouldUpdate();
}
void Animator::SetColorTransition(const sf::Color& _newColor)
{
	this->SetColorTransition(this->m_data.colorTransition[1], _newColor);
}


void Animator::SetRotation(float _start, float _end)
{
	this->m_data.rotation[0] = _start;
	this->m_data.rotation[1] = _end;
	this->m_usedAnims[ROTATION] = true;
	this->SetShouldUpdate();
}


sf::Vector2f Animator::GetGoTo(const sf::Vector2f& _startPos, const sf::Vector2f& _endPos, float _coefficient, Easing::Type _easing)
{
	float frameCoef = Easing::GetCoefficient(_coefficient, _easing);
	return sf::Vector2f(AniMath::Interpolate(_startPos.x, _endPos.x, _coefficient),
						AniMath::Interpolate(_startPos.y, _endPos.y, _coefficient));
}
sf::Vector2f Animator::GetGoTo(Easing::Type _easing)
{
	return GetGoTo(this->m_data.goTo[0], this->m_data.goTo[1], this->GetFrameCoefficient(), _easing);
}
sf::Vector2f Animator::GetGoTo(void)
{
	return this->GetGoTo(this->m_animEasings[GOTO]);
}


sf::Color Animator::GetColor(const sf::Color& _startColor, const sf::Color& _endColor, float _coefficient, Easing::Type _easing)
{
	float frameCoef = Easing::GetCoefficient(_coefficient, _easing);
	
	return sf::Color(	AniMath::InterpolateI(_startColor.r, _endColor.r, frameCoef),
						AniMath::InterpolateI(_startColor.g, _endColor.g, frameCoef),
						AniMath::InterpolateI(_startColor.b, _endColor.b, frameCoef),
						AniMath::InterpolateI(_startColor.a, _endColor.a, frameCoef));
}
sf::Color Animator::GetColor(Easing::Type _easing)
{
	return GetColor(this->m_data.colorTransition[0]	, this->m_data.colorTransition[1],
					this->GetFrameCoefficient()		, _easing);
}
sf::Color Animator::GetColor(void)
{
	return this->GetColor(this->m_animEasings[COLOR_TRANSITION]);
}


float Animator::GetRotation(float _start, float _end, float _coefficient, Easing::Type _easing)
{
	return AniMath::Interpolate(_start, _end, Easing::GetCoefficient(_coefficient, _easing));
}

float Animator::GetRotation(Easing::Type _easing)
{
	return GetRotation(	this->m_data.rotation[0]	, this->m_data.rotation[1],
						this->GetFrameCoefficient()	, _easing);
}

float Animator::GetRotation(void)
{
	return GetRotation(this->m_animEasings[ROTATION]);
}

}

// Animator v1.2.2