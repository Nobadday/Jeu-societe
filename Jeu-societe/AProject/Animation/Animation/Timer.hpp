#pragma once
#ifndef _INC_TIMER_HPP
#define _INC_TIMER_HPP

class DeltaClock
{
	protected:
		float m_timeElapsed = 0.0f;
		float m_speed = 1.0f;
		bool m_play = true;

	public:

		//Default constructor
		DeltaClock(void);
		DeltaClock(DeltaClock& _copy);

		DeltaClock(float _currentTime, float _speed = 1.0f, bool _play = true);

		void SyncTime(DeltaClock& _timer);

		void Update(float _deltaTime);

		void SetTime(float _seconds);
		void SetTime(float _minutes, float _seconds);
		void SetTime(float _minutes, float _seconds, float _miliseconds);
		void AddTime(float _seconds);

		void SetSpeed(float _value);
		void AddSpeed(float _value);

		void SetPause(bool _condition);
		void TogglePause(void);
		void SetReverse(bool _condition);

		void Restart(float _offset = 0.0f);

		float GetTimeElapsed(void);

		float GetSpeed(void);

		// Get time difference between this clock and _dtClock
		// thisClock - _dtClock
		float TimeDifference(DeltaClock& _dtClock);
		// Get time difference between this clock and _seconds
		// thisClock - _seconds
		float TimeDifference(float _seconds);


		bool IsPlaying(void);
		bool IsPaused(void);
		bool IsReversed(void);
		
		

		void operator+=(float _seconds);
		void operator-=(float _seconds);
		void operator=(float _seconds);
		//void operator+(float _seconds);
		//void operator-(float _seconds);

		//void operator==(float _seconds);

		operator float();
};


class Timer : public DeltaClock
{
	protected:
		float m_timeTarget = 0.0f;
	
	public:
		
		Timer(void);
		//Timer(Timer& _copy);
		Timer(float _timeTarget, float _speed = 1.0f, bool _play = true, float _currentTime = 0.0f);

		// Restarts the timer and apply the excess time as offset
		void RestartOffsetExcessTime(void);

		void SetTimeTarget(float _seconds, bool _restart = false);
		void End(void);

		bool IsFinished(void);

		float GetExcessTime(void);
		float GetRemainingTime(void);

		// Returns the coefficient of progression : time / targetTime
		float GetProgress(void);

		int GetLoopCount(void);

		float GetTimeTarget(void);

		// Get the real life remaining time using the speed of the timer
		float GetRemainingTimeAccurate(void);
		// Get the real life time using the speed of the timer
		float GetTimeTargetAccurate(void);
		
};

#endif
// Timer & DeltaClock C++ v1.0