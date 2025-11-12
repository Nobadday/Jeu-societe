#pragma once
#ifndef _INC_TIMER_HPP
#define _INC_TIMER_HPP


class DeltaClock
{
	protected:
		float m_timeElapsed;
		float m_speed;
		bool m_play;

	public:
		//Default constructor
		DeltaClock(void);
		DeltaClock(const DeltaClock& _copy);
		DeltaClock(float _currentTime, float _speed = 1.0f, bool _play = true);

		// Synchronise this clock with another one
		void SyncTime(DeltaClock& _clock);

		// Updates the DeltaClock by adding time
		void Update(float _deltaTime);

		void SetTime(float _seconds);
		void SetTime(float _minutes, float _seconds);
		void SetTime(float _hours, float _minutes, float _seconds);
		
		// Adds time regardless if it's paused or not and doesn't multiply by the speed,
		// if that's not the wanted effect, call Update() instead
		void AddTime(float _seconds);

		void SetSpeed(float _speed);
		void AddSpeed(float _value);

		void SetPause(bool _condition);
		void TogglePause(void);
		// Change the sign of the speed depending on the condition
		// if true, speed will be set to a negative value
		// else a positive value
		void SetReverse(bool _condition);
		// Inverses the sign of the current speed,
		// Positive speed will become negative and negative speed will become positive
		void ToggleReverse(void);

		void Restart(float _offset = 0.0f);

		// Get the current time stored in the clock
		float GetTimeElapsed(void);

		float GetSpeed(void);

		// Get time difference between this clock and _seconds
		// thisClock - _seconds
		float TimeDifference(float _seconds);

		
		// Returns True if the clock wants to run while it's speed ain't 0
		bool IsPlaying(void);
		// Returns True if the clock is paused OR it's speed is 0
		bool IsPaused(void);

		// Returns true if the clock wants to play
		bool IsWantingToPlay(void);
		// Returns true if the clock wants to be paused
		bool IsWantingToPause(void);
		

		// Returns True if the speed is negative
		bool IsReversed(void);


		void operator+=(float _seconds);
		void operator-=(float _seconds);
		void operator=(float _seconds);
		float operator+(float _seconds);
		float operator-(float _seconds);
		bool operator==(float _seconds);
		bool operator <=(float _seconds);
		bool operator >=(float _seconds);

		operator float();

	protected:
		// Virtual method that is called when the time is updated/modified/added
		virtual void OnTimeChange(void);
};


class Timer : public DeltaClock
{
	protected:
		float m_timeTarget;
	
	public:
		// Default constructor, creates a timer with a timeTarget of 0
		Timer(void);
		Timer(Timer& _copy);
		Timer(float _timeTarget, float _speed = 1.0f, bool _play = true, float _currentTime = 0.0f);

		// Restarts the timer and apply the excess time as offset
		void RestartOffsetExcessTime(void);


		void SetTimeTarget(float _seconds, bool _restart = false);
		// Set the current time to the target time, ends the timer
		void End(void);

		bool IsFinished(void);

		// Get the remaining time before the timer ends, if it exceeds it's end time, returns 0
		float GetRemainingTime(void);
		// Get the excess time added to the timer, only if it's finished otherwise returns 0
		float GetExcessTime(void);

		// Returns the coefficient of progression : time / targetTime
		float GetProgress(void);

		int GetLoopCount(void);

		float GetTimeTarget(void);

		// Get the "real life" remaining time dividing this by the speed of the timer
		float GetRemainingTimeAccurate(void);
		// Get the "real life" time target by dividing this by the speed of the timer
		float GetTimeTargetAccurate(void);
		
};

#endif

// DeltaClock & Timer C++ || v1.3.2