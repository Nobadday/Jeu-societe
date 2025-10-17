#pragma once

#ifndef _INC_ANIMATION_HANDLER_HPP
#define _INC_ANIMATION_HANDLER_HPP

#include "../Common.hpp"
#include "Timer.hpp"
#include "../Utilities/Math.hpp"


class Animation : private DeltaClock
{
	private:
		float m_frame = 0.0f;
		int m_frameCount = 0;
		// The duration of a single frame stored instead of FPS for optimisation purposes
		float m_frameTime = 0.0f;
		bool m_loop = false;

		bool m_shouldUpdate = false;

	public:
		//Default constructor
		Animation(void);
		// Copy constructor
		Animation(Animation& _copy);

		// Create a frame-based animation object
		Animation(int _frameCount, float _framerate, bool _loop = false, float _speed = 1.0f);
		// Create a time-based animation object
		Animation(float _durationSeconds, float _framerate = 24.0f, bool _loop = false, float _speed = 1.0f);


	#pragma region Modifiers
		// Frame-based animation
		void Modify(int _frameCount, float _framerate, bool _loop = false, float _speed = 1.0f);
		// Time-based animation object
		void Modify(float _durationSeconds, float _framerate, bool _loop = false, float _speed = 1.0f);
	
	#pragma endregion


	#pragma region Updates
		// Updates the animation, should be called every updates (automatically updates frames)
		void Update(float _deltaTime);

		// Updates the current frame relative to the amount of time elapsed
		// Function already called when needed to, you should not call this function
		void UpdateFrame(void);

		// Synchronise an animation with time
		void SyncTime(Animation& _syncer);
		// Synchronise an animation with it's current frames
		void SyncFrame(Animation& _syncer);
	#pragma endregion


	#pragma region SET/ADD
		// Set Current animation frame (starts at 0);
		// If the frame is different, shouldUpdate is set to True
		void SetFrame(float _frame);
		void SetFrame(int _frame);

		// Add set amount of frame
		void AddFrame(float _value);
		void AddFrame(int _value);

		// Set the amount of frames, CAN'T be 0
		void SetFrameCount(int _frameCount);
		// Add the amount of total frames
		void AddFrameCount(int _value);

		// Set the framerate, positive only (FPS)
		void SetFramerate(float _framerate);
		void SetFramerate(int _framerate);

		// Add Framerate (FPS)
		void AddFramerate(float _value);

		// Set animation Speed (negative for reverse animation)
		using DeltaClock::SetSpeed;
		// Add the speed to the animation
		using DeltaClock::AddSpeed;

		using DeltaClock::SetPause;

		// Set if the animation should be looped (True to loop)
		void SetLoop(bool _condition);
		// Toggle animation loop
		void ToggleLoop(void);

		// Set the animation to frame 0,
		// Animation Time can be offsetted in seconds
		void Restart(float _offsetSeconds = 0.0f);
		// Restart the animation and offset
		void RestartOffsetExcessTime(void);

		// End the animation (set to the last frame)
		void End(void);
		// End the animation (set to the last frame)
		void SetEndFrame(void);


		// Set ShouldUpdate to true: should tell animated objects to update themselves.
		void SetShouldUpdate(void);

		// Only set shouldUpdate to true if _condition is true
		void SetShouldUpdateProtected(bool _condition);

	#pragma endregion
		

	#pragma region GET
		// Returns the time elapsed multiplied by the current framerate
		// Time/(1/framerate)
		float GetCurrentFrameUncapped(void);

		// Get the current frame CAPPED as Float (starts at 0.0f, precise gets updated/capped every update)
		float GetCurrentFrame(void);

		// Get the current frame CAPPED as Int (starts at 0)
		int GetCurrentFrameInt(void);

		// Get the count of frames
		int GetFrameCount(void);

		// Get the current speed of the animation, reversed animations have negative speeds
		float GetSpeed(void);

		// Get the internal time of the animation, goes negative with reversed looped animations
		float GetTimeElapsed(void);

		// Get the current duration of one frame
		float GetFrameDuration(void);
		// Get the current duration of one frame divided by speed
		float GetFrameDurationSpeed(void);
		// Get the current duration of one frame divided by |speed|
		float GetFrameDurationSpeedPositive(void);

		// Get the current framerate framerate (FPS)
		float GetFramerate(void);
		// Get the current framerate multiplied by speed
		float GetFramerateSpeed(void);
		// Get the current framerate multiplied by |speed|
		float GetFramerateSpeedPositive(void);
		
		// Get currentFrame / frameCount
		float GetFrameCoefficient(void);

		// Get (currentFrame / frameCount) * 100
		float GetFramePercentage(void);
		

		// Returns the amounts of loops made through an looped animation
		int GetLoopCount(void);

		// Returns the duration of the animation in seconds unaffected by speed
		float GetDuration(void);
		// Returns the duration of the animation in seconds multiplied by speed
		// Can also be interpreted as real life duration
		float GetDurationSpeed(void);

		// Returns the remaining time before the animation ends (in seconds)
		float GetRemainingTime(void);
		// Returns the real life remaining time before the animation ends (in seconds)
		float GetRemainingTimeAccurate(void);
		// Returns the excess time if the animation ended (in seconds)
		// Used for precise time offset
		float GetExcessTime(void);

	#pragma endregion


	#pragma region IS-Boolean
		using DeltaClock::IsPlaying;

		using DeltaClock::IsPaused;

		using DeltaClock::IsReversed;

		// Returns true if the animation is looped
		bool IsLooped(void);

		// Returns true if the animation has reached it's last frame and the last frame has been displayed
		// If _forReversed is true : will work only for reversed animations
		// else, will work only for non-reversed animations
		// See the parameter-less overload for both checks
		bool IsFinished(bool _forReversed);
		// Returns true if the animation has reached it's last frame and the last frame has been displayed
		// Works for both reversed and non-reversed animations
		// if animation is reversed, finishes on frame 0
		// else, finishes normally
		bool IsFinished(void);


		//// Return true if the animation is on it's first frame
		//// If it's reversed, first frame is End frame
		//bool IsOnStartFrame(void);
		//// Return true if the animation is on it's last frame
		//// If it's reversed, last frame is First frame
		//bool IsOnEndFrame(void);

		// Returns true if the frame has changed since the last update and resets the boolean
		bool ShouldUpdate(void);

		// Returns true if the frame has changed since the last update
		// Use ShouldUpdate() instead for optimisation purposes
		bool ShouldUpdateFixed(void);
		
		// DEPRECATED, use ShouldUpdate() Instead !
		// Returns true if the frame has changed since the last update and resets the boolean
		bool ShouldUpdateOnce(void);

	#pragma endregion
};


#endif

// AnimationHandler C++ v2.0