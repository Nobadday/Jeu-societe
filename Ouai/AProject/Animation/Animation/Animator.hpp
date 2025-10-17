#pragma once

#ifndef _INC_ANIMATION_ANIMATOR_HPP
#define _INC_ANIMATION_ANIMATOR_HPP

#include "../Common.hpp"
#include "AnimationHandler.hpp"
#include "AnimationEasing.hpp"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>


namespace ANIMATION_NAMESPACE
{
class Animator : public Animation
{
	public:
		enum Animations
		{
			GOTO,
			COLOR_TRANSITION,
			//SCALE,

			ANIMATION_COUNT
		};
		Animator(void);
		Animator(float _durationSeconds, float _framerate, bool _loop, float _speed);


		bool AnimateObject(sf::Transformable& _object);
		bool AnimateObject(sf::Sprite& _object);
		//bool AnimateObject(sf::Text& _object);
		//bool AnimateObject(sf::Shape& _object);


		void ResetAnimation(void);
		//void ResetAnimation(Animations _animation);

		void SetAnimationEasing(Animations _animation, Easing::Type _easing);


		void SetGoTo(const sf::Vector2f& _startPos, const sf::Vector2f& _endPos);
		void SetGoTo(const sf::Vector2f& _startOrEnd, bool _start);
		void SetGoTo(const sf::Vector2f& _newPosition);
		void SetGoTo(const sf::Transformable& _object, const sf::Vector2f& _newPosition);

		void SetColorTransition(const sf::Color& _startColor, const sf::Color& _endColor);
		void SetColorTransition(const sf::Color& _newColor);

		sf::Vector2f GetGoTo(Easing::Type _easing);
		sf::Vector2f GetGoTo(void);
		

		sf::Color GetColorTransition(Easing::Type _easing);
		sf::Color GetColorTransition(void);
		
		//static sf::Color GetColorTransition(const sf::Color& _startColor, const sf::Color& _endColor, float _coefficient, Easing::Type _easing);

	private:
		struct AnimatorData
		{
			// [0] Start position, [1] End position
			sf::Vector2f goTo[2];
			sf::Color colorTransition[2];
			//sf::Vector2f scale[2];
		};
		AnimatorData m_data;
		bool m_usedAnims[ANIMATION_COUNT];
		Easing::Type m_animEasings[ANIMATION_COUNT];
};

}

#endif
// Animator v1.0