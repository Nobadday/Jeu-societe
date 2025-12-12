#pragma once
#ifndef _INC_ANIMATION_ANIMATOR_HPP
#define _INC_ANIMATION_ANIMATOR_HPP


#include "../Common.hpp"
#include "AnimationHandler.hpp"
#include "AnimationEasing.hpp"
#include "../Utilities/Math.hpp"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>



namespace ANIMATION_NAMESPACE
{
class Animator : public Animation
{
	private:
		struct AnimatorData
		{
			// [0] Start position, [1] End position
			sf::Vector2f goTo[2];
			sf::Color colorTransition[2];
			sf::Vector2f scale[2];
			// Rotation in degrees
			float rotation[2] = { 0 };
		};

	public:
		enum Animations
		{
			GOTO,
			COLOR_TRANSITION,
			SCALE,

			ROTATION,

			ANIMATION_COUNT
		};

	private:
		AnimatorData m_data;
		bool m_usedAnims[ANIMATION_COUNT];
		Easing::Type m_animEasings[ANIMATION_COUNT];

	public:
		Animator(void);
		Animator(float _durationSeconds, float _framerate, bool _loop, float _speed);


		bool AnimateObject(sf::Transformable& _object);
		bool AnimateObject(sf::Sprite& _object);
		//bool AnimateObject(sf::Text& _object);
		//bool AnimateObject(sf::Shape& _object);


		void ResetAnimation(void);


		void SetAnimationEasing(Animations _animation, Easing::Type _easing);


		void SetGoTo(const sf::Vector2f& _startPos, const sf::Vector2f& _endPos);
		void SetGoTo(const sf::Vector2f& _startOrEnd, bool _start);
		void SetGoTo(const sf::Vector2f& _newPosition);
		void SetGoTo(const sf::Transformable& _object, const sf::Vector2f& _newPosition);


		void SetColorTransition(const sf::Color& _startColor, const sf::Color& _endColor);
		void SetColorTransition(const sf::Color& _newColor);


		void SetScale(const sf::Vector2f& _startScale, const sf::Vector2f _endScale);
		void SetScale(const sf::Vector2f& _startOrEnd, bool _start);
		void SetScale(const sf::Vector2f& _newScale);
		void SetScale(const sf::Transformable& _object, const sf::Vector2f& _newScale);


		void SetRotation(float _start, float _end);
		void SetRotation(float _startOrEnd, bool _start);
		void SetRotation(float _newRotation);
		void SetRotation(const sf::Transformable& _object, float _newRotation);



		static sf::Vector2f GetGoTo(const sf::Vector2f& _startPos, const sf::Vector2f& _endPos, float _coefficient, Easing::Type _easing = Easing::Type::LINEAR);
		sf::Vector2f GetGoTo(Easing::Type _easing);
		sf::Vector2f GetGoTo(void);
		

		static sf::Color GetColor(const sf::Color& _startColor, const sf::Color& _endColor, float _coefficient, Easing::Type _easing = Easing::Type::LINEAR);
		sf::Color GetColor(Easing::Type _easing);
		sf::Color GetColor(void);
		

		static sf::Vector2f GetScale(const sf::Vector2f& _startScale, const sf::Vector2f& _endScale, float _coefficient, Easing::Type _easing = Easing::Type::LINEAR);
		sf::Vector2f GetScale(Easing::Type _easing);
		sf::Vector2f GetScale(void);
		

		static float GetRotation(float _start, float _end, float _coefficient, Easing::Type _easing = Easing::Type::LINEAR);
		float GetRotation(Easing::Type _easing);
		float GetRotation(void);

};

}

#endif
// Animator SFML 2.6.2 || v1.3