#include "AnimatedObject.hpp"


AnimatedObject::AnimatedObject(void) :
m_texture (NULL),
m_animationSelected (-1)
{
}

bool AnimatedObject::Update(float _deltaTime)
{
	if (this->m_animationSelected != -1)
	{
		this->m_animationHandler.Update(_deltaTime);
		if (this->m_animationHandler.ShouldUpdateOnce())
		{
			this->UpdateFrame();
			return true;
		}
	}
	return false;
}

void AnimatedObject::UpdateFrame(void)
{
	
}

void AnimatedObject::SetTexture(TextureAnimated& _texture, bool _keepAnimation)
{
	std::string lastAnim;
	if (_keepAnimation)
	{
		lastAnim = this->m_texture->GetAnimation(this->m_animationSelected)->m_name;
	}
	this->m_texture = &_texture;

	if (_keepAnimation)
	{
		this->SetAnimation(lastAnim);
	}
	else
	{
		if (!this->m_texture->AnimationExists(this->m_animationSelected))
		{
			this->SetAnimation(0, true);
		}
		else
		{
			this->UpdateFrame();
		}
	}
}

void AnimatedObject::SetAnimation(int _animationIndex, bool _resetAnimation)
{
	if (_animationIndex <= -1)
	{
		this->m_animationSelected = -1;
	}
	else if (this->m_texture != NULL)
	{
		if (_animationIndex != this->m_animationSelected)
		{
			this->m_animationSelected = _animationIndex;
			AnimationProperties* anim = this->m_texture->GetAnimation(this->m_animationSelected);
			this->m_animationHandler.SetFrameCount(anim->m_frameCount);
			this->m_animationHandler.SetFramerate(anim->m_framerate);
			this->m_animationHandler.SetLoop(anim->m_loop);
			this->m_animationHandler.Restart();
		}
		else if (_resetAnimation)
		{
			this->m_animationHandler.Restart();
		}
	}
	this->UpdateFrame();
}

void AnimatedObject::SetAnimation(const std::string& _animationName, bool _resetAnimation)
{
	if (this->m_texture != NULL)
	{
		int animIndex = this->m_texture->FindAnimationIndex(_animationName);
		if (animIndex != -1)
		{
			this->SetAnimation(animIndex, _resetAnimation);
		}
	}
}

void AnimatedObject::AddAnimation(int _value, bool _wrap, bool _resetAnimation)
{
	if (this->m_texture != NULL)
	{
		int index = this->m_animationSelected + _value;
		if (_wrap)
		{
			index = anim::AniMath::ModuloPositiveI(index, this->m_texture->GetAnimationCount());
		}
		this->SetAnimation(index, _resetAnimation);
	}
}

void AnimatedObject::RestartAnimation(float _offset)
{
	this->m_animationHandler.Restart(_offset);
	this->UpdateFrame();
}

bool AnimatedObject::HasAnimation(void)
{
	return this->m_animationSelected != -1;
}

AnimationProperties* AnimatedObject::GetAnimationProperties(void)
{
	return this->m_texture->GetAnimation(this->m_animationSelected);
}
// AnimatedObject v1.0