#pragma once
#ifndef _INC_ANIMATIONOBJECT_HPP
#define _INC_ANIMATIONOBJECT_HPP


#include "../Common.hpp"
#include "../Animation/AnimationHandler.hpp"
#include "./TextureAnimated.hpp"

class AnimationPRO : public Animation
{
	public:
		AnimationPRO(void) : Animation() {};

	protected:
		using Animation::Modify;
		using Animation::SetFrameCount;
		using Animation::AddFrameCount;
		using Animation::SetDuration;
		using Animation::SetFramerate;
		using Animation::AddFramerate;
		using Animation::SetShouldUpdate;
		using Animation::SetShouldUpdateProtected;

};

class AnimatedObject : public AnimationPRO
{
	protected:
		TextureAnimated* m_texture;
		int m_animationSelected;

	public:
		AnimatedObject(void);

		// Set the animation of the sprite using the index
		// If _resetAnimation is true, if the animation is already selected, resets that animation
		void SetAnimation(int _animationIndex, bool _resetAnimation = true);
		// Set the animation of the sprite using the name
		// If _resetAnimation is true, if the animation is already selected, resets that animation
		void SetAnimation(const std::string& _animationName, bool _resetAnimation = true);

		// Go foward/backwards an animation
		// if _warp is true, will wrap
		// If _resetAnimation is true, if the animation is already selected, resets that animation
		void AddAnimation(int _value, bool _wrap = false, bool _resetAnimation = false);


		// Returns true if the current animation selected is valid
		bool HasAnimation(void);


	protected:
		// If keepAnimation true; set's animation to last anim
		void SetTexture(TextureAnimated& _texture, bool _keepAnimation = false);

		virtual void AnimationChanged(void);
};

#endif
// AnimatedObject || v1.2.2