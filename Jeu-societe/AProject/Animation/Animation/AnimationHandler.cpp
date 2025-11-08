#include "AnimationHandler.hpp"


#pragma region Animation Class


#pragma region Constructors

Animation::Animation(void) :
m_frame			(0.0f),
m_frameCount	(0),
m_frameTime		(ANIMATION_DEFAULT_FPS_SECONDS),
m_loop			(false),
m_shouldUpdate	(false)
{

}
Animation::Animation(Animation& _copy) : DeltaClock(_copy),
m_frame			(_copy.m_frame),
m_frameCount	(_copy.m_frameCount),
m_frameTime		(_copy.m_frameTime),
m_loop			(_copy.m_loop),
m_shouldUpdate	(_copy.m_shouldUpdate)
{

}
Animation::Animation(int _frameCount, float _framerate, bool _loop, float _speed)
{
	this->Modify(_frameCount, _framerate, _loop, _speed);
}
Animation::Animation(float _durationSeconds, float _framerate, bool _loop, float _speed)
{
	this->Modify(_durationSeconds, _framerate, _loop, _speed);
}

#pragma endregion


#pragma region Modifiers

void Animation::Modify(int _frameCount, float _framerate, bool _loop, float _speed)
{
	this->m_frame = 0.0f;
	this->m_frameCount = _frameCount;
	this->m_loop = _loop;
	this->m_speed = _speed;

	this->m_shouldUpdate = true;

	this->SetFramerate(_framerate);
	this->OnFrameChange();
}
void Animation::Modify(int _frameCount, float _framerate, bool _loop)
{
	this->m_frame = 0.0f;
	this->m_frameCount = _frameCount;
	this->m_loop = _loop;

	this->m_shouldUpdate = true;

	this->SetFramerate(_framerate);
	this->OnFrameChange();
}
void Animation::Modify(float _durationSeconds, float _framerate, bool _loop, float _speed)
{
	this->Modify((int)anim::AniMath::SecondsToFrameTime(_durationSeconds, _framerate), _framerate, _loop, _speed);
}
void Animation::Modify(float _durationSeconds, float _framerate, bool _loop)
{
	this->m_frame = 0.0f;
	this->m_frameCount = (int)anim::AniMath::SecondsToFrameTime(_durationSeconds, _framerate);
	this->m_loop = _loop;

	this->m_shouldUpdate = true;

	this->SetFramerate(_framerate);
	this->OnFrameChange();
}

#pragma endregion


#pragma region Updates

void Animation::Update(float _deltaTime)
{
	// Don't add time/update if the animation is PAUSED
	// AND if the animation isn't looped & finished
	if (this->m_play && !(this->IsFinished() && !this->m_loop))
	{
		// TODO : Appeler DeltaClock avec onTimeUpdate
		this->m_timeElapsed += _deltaTime * this->m_speed;
		this->UpdateFrame();
	}
}

void Animation::UpdateFrame(void)
{
	if (this->m_frameCount <= 0)
	{
		return;
	}

	float newFrame = this->GetCurrentFrameUncapped();

	if (this->m_loop)
	{
		// Frame will always have proper values
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
			// -1 to convert number to index
			newFrame = (float)(m_frameCount - 1);
		}
	}

	// Checks for frameUpdates
	if (this->m_frame != newFrame)
	{
		bool frameChanged = (int)this->m_frame != (int)newFrame;

		this->m_frame = newFrame;
		if (frameChanged)
		{
			this->m_shouldUpdate = true;
			this->OnFrameChange();
		}
	}
}

void Animation::SyncTime(Animation& _syncer)
{
	this->DeltaClock::SyncTime(_syncer);
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
	if (_frameCount < 0)
	{
		_frameCount = 0;
	}
	this->m_frameCount = _frameCount;
	this->UpdateFrame();
}
void Animation::AddFrameCount(int _value)
{
	this->SetFrameCount(this->m_frameCount + _value);
}

void Animation::SetDuration(float _seconds, float _framerate)
{
	this->SetFrameCount((int)anim::AniMath::SecondsToFrameTime(_seconds, _framerate));
}
void Animation::SetDuration(float _seconds)
{
	this->SetDuration(_seconds, this->GetFramerate());
}

void Animation::SetFramerate(float _framerate)
{
	if (_framerate < ANIMATION_MINIMUM_FPS)
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
	this->SetFramerate(this->GetFramerate() + _value);
}
void Animation::AddFramerate(int _value)
{
	this->AddFramerate((float)_value);
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


#pragma endregion


#pragma region GET

float Animation::GetCurrentFrameUncapped(void)
{
	if (this->m_frameTime > 0.0f)
	{
		return this->m_timeElapsed / this->m_frameTime;
	}
	// Couldn't get precise frame time -> return last frame
	return this->m_frame;
}

int Animation::GetCurrentFrame(void)
{
	return (int)this->m_frame;
}
float Animation::GetCurrentFramePrecise(void)
{
	return this->m_frame;
}

int Animation::GetFrameCount(void)
{
	return this->m_frameCount;
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
		return this->m_frameTime / std::fabsf(this->m_speed);
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
		return this->GetFramerate() / std::fabsf(this->m_speed);
	}
	return 0.0f;
}

float Animation::GetProgress(void)
{
	return this->GetFrameCoefficient();
}
float Animation::GetFrameCoefficient(void)
{
	if (this->m_frameCount > 1)
	{
		// Troncate the float values to truely be frame by frame
		return (float)((float)this->GetCurrentFrame() / (float)(this->m_frameCount - 1));
	}
	return 1.0f;
}
float Animation::GetFramePercentage(void)
{
	return this->GetFrameCoefficient() * 100.0f;
}


int Animation::GetLoopCount(void)
{
	if (this->m_frameCount > 0)
	{
		if (this->IsReversed())
		{
			// Frames uncapped will be negatives
			return (int)this->GetCurrentFrameUncapped() / -this->m_frameCount;
		}
		return (int)this->GetCurrentFrameUncapped() / this->m_frameCount;
	}
	return 1;
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
	float time = this->GetDurationSpeed() - std::fabsf(this->m_timeElapsed);
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
	float time = std::fabsf(this->m_timeElapsed) - this->GetDurationSpeed();
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

bool Animation::IsOnStartFrame(void)
{
	if (this->IsReversed())
	{
		return this->GetCurrentFrame() == (this->m_frameCount - 1);
	}
	return this->GetCurrentFrame() == 0;
}
bool Animation::IsOnEndFrame(void)
{
	if (this->IsReversed())
	{
		return this->GetCurrentFrame() == 0;
	}
	return this->GetCurrentFrame() == (this->m_frameCount - 1);
}

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

#pragma endregion

void Animation::OnFrameChange(void)
{
	// Virtual method, does nothing here
}

#pragma endregion Animation Class End

// AnimationHandler C++ || v2.2