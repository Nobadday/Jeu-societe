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
		virtual void UpdateFrame(void) = 0;


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
		TextureAnimated::AnimationProperties* GetAnimationProperties(void);

		// Please call UpdateFrame after modifying attributes of the animation
		Animation& GetAnimationHandler(void);
};


class AnimatedObj : public AnimationPRO
{
	protected:
		TextureAnimated* m_texture;
		int m_animationSelected;

	public:
		AnimatedObj(void);

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
// AnimatedObject || v1.2