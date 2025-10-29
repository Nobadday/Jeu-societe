#include "AnimationHandler.hpp"

#define ANIMATION_MINIMUM_FPS 0.01f

#pragma region Animation Class
Animation::Animation(void)
{
	//this->m_frame = 0.0f;
	//this->m_frameCount = 0;
	//this->m_frameTime = 0.0f;
	//this->m_loop = false;

	//this->m_shouldUpdate = false;
}

Animation::Animation(Animation& _copy) : DeltaClock(_copy)
{
	this->m_frame = _copy.m_frame;
	this->m_frameCount = _copy.m_frameCount;
	this->m_frameTime = _copy.m_frameTime;
	this->m_loop = _copy.m_loop;

	this->m_shouldUpdate = _copy.m_shouldUpdate;
}

Animation::Animation(int _frameCount, float _framerate, bool _loop, float _speed)
{
	this->Modify(_frameCount, _framerate, _loop, _speed);
}

Animation::Animation(float _durationSeconds, float _framerate, bool _loop, float _speed)
{
	this->Modify(_durationSeconds, _framerate, _loop, _speed);
}


#pragma region Modifiers

void Animation::Modify(int _frameCount, float _framerate, bool _loop, float _speed)
{
	this->m_frame = 0.0f;
	this->m_frameCount = _frameCount;
	this->m_loop = _loop;
	this->m_speed = _speed;

	this->SetFramerate(_framerate);
	
	this->m_shouldUpdate = true;
}
void Animation::Modify(float _durationSeconds, float _framerate, bool _loop, float _speed)
{
	this->Modify((int)anim::AniMath::SecondsToFrameTime(_durationSeconds, _framerate), _framerate, _loop, _speed);
}

#pragma endregion


#pragma region Updates

void Animation::Update(float _deltaTime)
{
	// Don't add time/update if the animation is PAUSED
	// AND if the animation isn't looped & finished
	if (this->m_play && !(this->IsFinished() && !this->m_loop))
	{
		this->m_timeElapsed += _deltaTime * this->m_speed;
		this->UpdateFrame();
	}
	//this->UpdateFrame();
}

void Animation::UpdateFrame(void)
{
	float newFrame = this->GetCurrentFrameUncapped();

	if (this->m_loop)
	{
		// Frame will always have proper values
		// Modulo always positive :
		newFrame = anim::AniMath::ModuloPositiveF(newFrame, (float)m_frameCount);
	}
	else
	{
		// Checks if frame value is out of range, restricts the value between 0 and frameCount
		if (newFrame < 0.0f)
		{
			newFrame = 0.0f;
		}
		else if (newFrame > m_frameCount - 1)
		{
			// ( Convert number to index -> [-1])
			newFrame = (float)(m_frameCount - 1);
		}
	}

	// Checks shouldUpdate
	if (this->m_frame != newFrame)
	{
		if ((int)this->m_frame != (int)newFrame)
		{
			this->m_shouldUpdate = true;
		}
		this->m_frame = newFrame;
	}
}

void Animation::SyncTime(Animation& _syncer)
{
	//this->DeltaClock::SyncTime(_syncer);
	this->UpdateFrame();
}
void Animation::SyncFrame(Animation& _syncer)
{
	this->SetFrame(_syncer.m_frame);
}

#pragma endregion


#pragma region SET/ADD

void Animation::SetFrame(float _frame)
{
	this->m_timeElapsed = _frame * this->m_frameTime;
	this->UpdateFrame();
}
void Animation::SetFrame(int _frame)
{
	this->SetFrame((float)_frame);
}

void Animation::AddFrame(float _value)
{
	this->SetFrame(this->m_frame + _value);
}
void Animation::AddFrame(int _value)
{
	this->AddFrame((float)_value);
}

void Animation::SetFrameCount(int _frameCount)
{
	this->m_frameCount = (_frameCount)+((_frameCount < 1) * ((_frameCount * -1) + 1));
	this->UpdateFrame();
}
void Animation::AddFrameCount(int _value)
{
	this->SetFrameCount(this->m_frameCount + _value);
}

void Animation::SetFramerate(float _framerate)
{
	if (_framerate <= 0.0f)
	{
		_framerate = ANIMATION_MINIMUM_FPS;
	}
	this->m_frameTime = anim::AniMath::FPSToFrameTime(_framerate);
	this->SetFrame(this->m_frame);
}
void Animation::SetFramerate(int _framerate)
{
	this->SetFramerate((float)_framerate);
}

void Animation::AddFramerate(float _value)
{
	this->m_frameTime += anim::AniMath::FPSToFrameTime(_value);
	this->SetFrame(this->m_frame);
}

void Animation::SetLoop(bool _condition)
{
	this->m_loop = _condition;
	if (!this->m_loop)
	{
		// Exit out of loop, set the correct time frame to not get capped
		this->SetFrame(this->GetCurrentFrameUncapped() / (float)(1 + this->GetLoopCount()));
	}
}

void Animation::ToggleLoop(void)
{
	this->SetLoop(!this->m_loop);
}

void Animation::Restart(float _offsetSeconds)
{
	this->DeltaClock::Restart(_offsetSeconds);
	this->UpdateFrame();
}
void Animation::RestartOffsetExcessTime(void)
{
	if (this->m_loop)
	{
		this->Restart(this->GetExcessTime() / (float)(1 + this->GetLoopCount()));
	}
	else
	{
		this->Restart(this->GetExcessTime());
	}
}

void Animation::End(void)
{
	this->SetFrame(this->m_frameCount + 1);
}
void Animation::SetEndFrame(void)
{
	this->End();
}

void Animation::SetShouldUpdate(void)
{
	this->m_shouldUpdate = true;
}
void Animation::SetShouldUpdateProtected(bool _condition)
{
	this->m_shouldUpdate = this->m_shouldUpdate || _condition;
}

#pragma endregion


#pragma region GET

float Animation::GetCurrentFrameUncapped(void)
{
	if (this->m_frameTime != 0.0f)
	{
		return this->m_timeElapsed / this->m_frameTime;
	}
	// Couldn't get precise frame time -> return last frame
	return this->m_frame;
}

float Animation::GetCurrentFrame(void)
{
	return this->m_frame;
}

int Animation::GetCurrentFrameInt(void)
{
	return (int)this->m_frame;
}

int Animation::GetFrameCount(void)
{
	return this->m_frameCount;
}

float Animation::GetSpeed(void)
{
	return this->m_speed;
}

float Animation::GetFrameDuration(void)
{
	return this->m_frameTime;
}
float Animation::GetFrameDurationSpeed(void)
{
	if (this->m_speed != 0.0f)
	{
		return this->m_frameTime / this->m_speed;
	}
	return 0.0f;
}
float Animation::GetFrameDurationSpeedPositive(void)
{
	if (this->m_speed != 0.0f)
	{
		return this->m_frameTime / fabsf(this->m_speed);
	}
	return 0.0f;
}

float Animation::GetFramerate(void)
{
	return anim::AniMath::FrameTimeToFPS(this->m_frameTime);
}
float Animation::GetFramerateSpeed(void)
{
	if (this->m_speed != 0.0f)
	{
		return this->GetFramerate() / this->m_speed;
	}
	return 0.0f;
}
float Animation::GetFramerateSpeedPositive(void)
{
	if (this->m_speed != 0.0f)
	{
		return this->GetFramerate() / fabsf(this->m_speed);
	}
	return 0.0f;
}

float Animation::GetFrameCoefficient(void)
{
	if (this->m_frameCount > 1)
	{
		// Troncate the float values to truely be frame by frame
		return (float)((float)this->GetCurrentFrameInt() / (float)(this->m_frameCount - 1));
	}
	return 1.0f;
}
float Animation::GetFramePercentage(void)
{
	return this->GetFrameCoefficient() * 100.0f;
}


int Animation::GetLoopCount(void)
{
	if (this->IsReversed())
	{
		// Frames uncapped will be negatives
		return (int)this->GetCurrentFrameUncapped() / -this->m_frameCount;
	}
	return (int)this->GetCurrentFrameUncapped() / this->m_frameCount;
}

float Animation::GetDuration(void)
{
	return (float)this->m_frameCount * this->m_frameTime;
}
float Animation::GetDurationSpeed(void)
{
	return (float)this->m_frameCount * this->GetFrameDurationSpeedPositive();
}

float Animation::GetRemainingTime(void)
{
	float time = this->GetDurationSpeed() - fabsf(this->m_timeElapsed);
	if (time < 0.0f)
	{
		time = 0.0f;
	}
	return time;
}
float Animation::GetRemainingTimeAccurate(void)
{
	if (this->m_speed != 0.0f)
	{
		return this->GetRemainingTime() / this->m_speed;
	}
	return 0.0f;
}
float Animation::GetExcessTime(void)
{
	float time = fabsf(this->m_timeElapsed) - this->GetDurationSpeed();
	if (time < 0.0f)
	{
		time = 0.0f;
	}
	return time;
}

#pragma endregion


#pragma region IS-Boolean

bool Animation::IsLooped(void)
{
	return this->m_loop;
}

bool Animation::IsFinished(bool _forReversed)
{
	if (_forReversed)
	{
		return this->GetCurrentFrameUncapped() <= 0.0f;
	}
	// Do not reduce framecount, animation must end after the frame ends
	return this->GetCurrentFrameUncapped() >= m_frameCount;
}
bool Animation::IsFinished(void)
{
	return this->IsFinished(this->IsReversed());
}

//bool Animation::IsOnStartFrame(void)
//{
//	if (this->IsReversed())
//	{
//		return this->GetCurrentFrameInt() == this->m_frameCount-1;
//	}
//	return this->GetCurrentFrameInt() == 0;
//}
//
//bool Animation::IsOnEndFrame(void)
//{
//	if (this->IsReversed())
//	{
//		return this->GetCurrentFrameInt() == 0;
//	}
//	return this->GetCurrentFrameInt() == this->m_frameCount - 1;
//}

bool Animation::ShouldUpdate(void)
{
	if (this->m_shouldUpdate)
	{
		this->m_shouldUpdate = false;
		return true;
	}
	return false;
}

bool Animation::ShouldUpdateFixed(void)
{
	return this->m_shouldUpdate;
}

bool Animation::ShouldUpdateOnce(void)
{
	return this->ShouldUpdate();
}



#pragma endregion

#pragma endregion Animation Class End

// AnimationHandler C++ v2.0