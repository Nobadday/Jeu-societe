#include "Timer.hpp"

#pragma region DeltaClock
#pragma region Constructors
DeltaClock::DeltaClock(void)
{
	this->m_timeElapsed = 0.0f;
	this->m_speed = 1.0f;
	this->m_play = true;
}
DeltaClock::DeltaClock(DeltaClock& _copy)
{
	this->m_timeElapsed = _copy.m_timeElapsed;
	this->m_speed = _copy.m_speed;
	this->m_play = _copy.m_play;
}

DeltaClock::DeltaClock(float _currentTime, float _speed, bool _play)
{
	this->m_timeElapsed = _currentTime;
	this->m_speed = _speed;
	this->m_play = _play;
}

#pragma endregion

void DeltaClock::SyncTime(DeltaClock& _timer)
{
	this->m_timeElapsed = _timer.m_timeElapsed;
}

void DeltaClock::Update(float _deltaTime)
{
	if (this->m_play)
	{
		this->m_timeElapsed += _deltaTime * this->m_speed;
	}
}

#pragma region Set/Add
void DeltaClock::SetTime(float _seconds)
{
	this->m_timeElapsed = _seconds;
}

void DeltaClock::SetTime(float _minutes, float _seconds)
{
	this->m_timeElapsed = (_minutes * 60.0f) + _seconds;
}

void DeltaClock::SetTime(float _minutes, float _seconds, float _miliseconds)
{
	this->m_timeElapsed = (_minutes * 60.0f) + _seconds + (_miliseconds/1000.0f);
}

void DeltaClock::AddTime(float _seconds)
{
	this->m_timeElapsed += _seconds;
}

void DeltaClock::SetSpeed(float _value)
{
	this->m_speed = _value;
}

void DeltaClock::AddSpeed(float _value)
{
	this->m_speed += _value;
}

#pragma endregion

void DeltaClock::Restart(float _offset)
{
	this->m_timeElapsed = 0.0f + _offset;
}

#pragma region Conditions
void DeltaClock::SetPause(bool _condition)
{
	this->m_play = !_condition;
}

void DeltaClock::TogglePause(void)
{
	this->m_play = !this->m_play;
}

void DeltaClock::SetReverse(bool _condition)
{
	if (this->IsReversed() != _condition)
	{
		this->m_speed = -this->m_speed;
	}
}
#pragma endregion

#pragma region Get
float DeltaClock::GetTimeElapsed(void)
{
	return this->m_timeElapsed;
}

float DeltaClock::GetSpeed(void)
{
	return this->m_speed;
}

float DeltaClock::TimeDifference(DeltaClock& _dtClock)
{
	return this->m_timeElapsed - _dtClock.m_timeElapsed;
}
float DeltaClock::TimeDifference(float _seconds)
{
	return this->m_timeElapsed - _seconds;
}
#pragma endregion

#pragma region Is Conditions
bool DeltaClock::IsPlaying(void)
{
	return this->m_play && this->m_speed != 0.0f;
}

bool DeltaClock::IsPaused(void)
{
	return !this->IsPlaying();
}

bool DeltaClock::IsReversed(void)
{
	return this->m_speed < 0.0f;
}

void DeltaClock::operator+=(float _seconds)
{
	this->AddTime(_seconds);
}

void DeltaClock::operator-=(float _seconds)
{
	this->AddTime(-_seconds);
}

void DeltaClock::operator=(float _seconds)
{
	this->SetTime(_seconds);
}

DeltaClock::operator float()
{
	return this->m_timeElapsed;
}

#pragma endregion

#pragma endregion // DeltaTime

#pragma region Timer
Timer::Timer()
{
	this->m_timeTarget = 0.0f;
}

Timer::Timer(float _timeTarget, float _speed, bool _play, float _currentTime) : DeltaClock(_currentTime, _speed, _play)
{
	this->m_timeTarget = _timeTarget;
}

void Timer::RestartOffsetExcessTime(void)
{
	this->DeltaClock::Restart(this->GetExcessTime());
}

void Timer::SetTimeTarget(float _seconds)
{
	this->m_timeTarget = _seconds;
}

void Timer::End(void)
{
	this->SetTime(this->m_timeTarget);
}

bool Timer::IsFinished(void)
{
	return this->m_timeElapsed >= this->m_timeTarget;
}

float Timer::GetExcessTime(void)
{
	if (this->IsFinished())
	{
		return this->m_timeElapsed - this->m_timeTarget;
	}
	return 0.0f;
}
float Timer::GetRemainingTime(void)
{
	if (!this->IsFinished())
	{
		return this->m_timeTarget - this->m_timeElapsed;
	}
	return 0.0f;
}

float Timer::GetProgress(void)
{
	if (this->m_timeTarget != 0.0f)
	{
		return this->m_timeElapsed / this->m_timeTarget;
	}
	return 1.0f;
}

int Timer::GetLoopCount(void)
{
	return (int)this->GetProgress();
}

float Timer::GetTimeTarget(void)
{
	return this->m_timeTarget;
}

float Timer::GetRemainingTimeAccurate(void)
{
	if (this->m_speed != 0.0f)
	{
		return this->GetRemainingTime() / this->m_speed;
	}
	return 0.0f;
}

float Timer::GetTimeTargetAccurate(void)
{
	if (this->m_speed != 0.0f)
	{
		return this->m_timeTarget / this->m_speed;
	}
	return 0.0f;
}
#pragma endregion