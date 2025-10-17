#include "Animator.hpp"

namespace ANIMATION_NAMESPACE
{

Animator::Animator(void) : Animation()
{
	this->ResetAnimation();
}

Animator::Animator(float _durationSeconds, float _framerate, bool _loop, float _speed) : Animation(_durationSeconds, _framerate, _loop, _speed)
{
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
			_object.setColor(this->GetColorTransition());
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


sf::Vector2f Animator::GetGoTo(Easing::Type _easing)
{
	float frameCoef = Easing::GetCoefficient(this->GetFrameCoefficient(), _easing);
	return sf::Vector2f(this->m_data.goTo[0].x + ((this->m_data.goTo[1].x - this->m_data.goTo[0].x) * frameCoef),
						this->m_data.goTo[0].y + ((this->m_data.goTo[1].y - this->m_data.goTo[0].y) * frameCoef));
}
sf::Vector2f Animator::GetGoTo(void)
{
	return this->GetGoTo(this->m_animEasings[GOTO]);
}


sf::Color Animator::GetColorTransition(Easing::Type _easing)
{
	float frameCoef = Easing::GetCoefficient(this->GetFrameCoefficient(), _easing);
	sf::Color& startColor = this->m_data.colorTransition[0];
	sf::Color& endColor = this->m_data.colorTransition[1];
	int colorInt[4] = { 0 };
	colorInt[0] = (int)(startColor.r + ((endColor.r - startColor.r) * frameCoef));
	colorInt[1] = (int)(startColor.g + ((endColor.g - startColor.g) * frameCoef));
	colorInt[2] = (int)(startColor.b + ((endColor.b - startColor.b) * frameCoef));
	colorInt[3] = (int)(startColor.a + ((endColor.a - startColor.a) * frameCoef));
	return sf::Color(colorInt[0], colorInt[1], colorInt[2], colorInt[3]);
}
sf::Color Animator::GetColorTransition(void)
{
	return this->GetColorTransition(this->m_animEasings[COLOR_TRANSITION]);
}

}
// Animator v1.0