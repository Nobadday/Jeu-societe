#pragma once
#ifndef _INC_ANIMATION_CLASS_HPP
#define _INC_ANIMATION_CLASS_HPP

#include "../Common.hpp"
#include "../Animation/AnimationHandler.hpp"
#include "./TextureAnimated.hpp"


class AnimatedObject
{
	protected:
		TextureAnimated* m_texture;

		Animation m_animationHandler;
		int m_animationSelected;
	
		// If keepAnimation true; set's animation to last anim
		void SetTexture(TextureAnimated& _texture, bool _keepAnimation = false);

	public:
		AnimatedObject(void);

		// Update the animation returns true if the frame changed
		bool Update(float _deltaTime);
		virtual void UpdateFrame(void);


		// Set the animation of the sprite using the index
		// If _resetAnimation is true, if the animation is already selected, resets that animation
		void SetAnimation(int _animationIndex, bool _resetAnimation = true);
		// Set the animation of the sprite using the name
		// If _resetAnimation is true, if the animation is already selected, resets that animation
		void SetAnimation(const std::string& _animationName, bool _resetAnimation = true);
		void AddAnimation(int _value, bool _wrap = false, bool _resetAnimation = false);

		void RestartAnimation(float _offset = 0.0f);

		// Returns true if the current animation selected is valid
		bool HasAnimation(void);

		// Get the current animation properties
		AnimationProperties* GetAnimationProperties(void);

		// Please call UpdateFrame after modifying attributes of the animation
		Animation& GetAnimationHandler(void);
};

#endif
// AnimatedObject v1.1