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
		if (this->m_usedAnims[SCALE])
		{
			_object.setScale(this->GetScale());
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
		if (this->m_usedAnims[SCALE])
		{
			_object.setScale(this->GetScale());
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


void Animator::SetScale(const sf::Vector2f& _startScale, const sf::Vector2f _endScale)
{
	this->m_data.scale[0] = _startScale;
	this->m_data.scale[1] = _endScale;
	this->m_usedAnims[SCALE] = true;
	this->SetShouldUpdate();
}
void Animator::SetScale(const sf::Vector2f& _startOrEnd, bool _start)
{
	this->m_data.scale[_start] = _startOrEnd;
	this->m_usedAnims[SCALE] = true;
	this->SetShouldUpdate();
}
void Animator::SetScale(const sf::Vector2f& _newScale)
{
	this->SetScale(this->m_data.scale[1], _newScale);
}
void Animator::SetScale(const sf::Transformable& _object, const sf::Vector2f& _newScale)
{
	this->SetScale(_object.getScale(), _newScale);
}


void Animator::SetRotation(float _start, float _end)
{
	this->m_data.rotation[0] = _start;
	this->m_data.rotation[1] = _end;
	this->m_usedAnims[ROTATION] = true;
	this->SetShouldUpdate();
}
void Animator::SetRotation(float _startOrEnd, bool _start)
{
	this->m_data.rotation[_start] = _startOrEnd;
	this->m_usedAnims[ROTATION] = true;
	this->SetShouldUpdate();
}
void Animator::SetRotation(float _newRotation)
{
	this->SetRotation(this->m_data.rotation[1], _newRotation);
}
void Animator::SetRotation(const sf::Transformable& _object, float _newRotation)
{
	this->SetRotation(_object.getRotation(), _newRotation);
}


sf::Vector2f Animator::GetGoTo(const sf::Vector2f& _startPos, const sf::Vector2f& _endPos, float _coefficient, Easing::Type _easing)
{
	float frameCoef = Easing::GetCoefficient(_coefficient, _easing);
	return sf::Vector2f(AniMath::Interpolate(_startPos.x, _endPos.x, frameCoef),
						AniMath::Interpolate(_startPos.y, _endPos.y, frameCoef));
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


sf::Vector2f Animator::GetScale(const sf::Vector2f& _startScale, const sf::Vector2f& _endScale, float _coefficient, Easing::Type _easing)
{
	float frameCoef = Easing::GetCoefficient(_coefficient, _easing);
	return sf::Vector2f(AniMath::Interpolate(_startScale.x, _endScale.x, frameCoef),
						AniMath::Interpolate(_startScale.y, _endScale.y, frameCoef));
}
sf::Vector2f Animator::GetScale(Easing::Type _easing)
{
	return GetScale(this->m_data.scale[0], this->m_data.scale[1], this->GetFrameCoefficient(), _easing);
}
sf::Vector2f Animator::GetScale(void)
{
	return this->GetScale(this->m_animEasings[SCALE]);
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

// Animator SFML 2.6.2 || v1.3