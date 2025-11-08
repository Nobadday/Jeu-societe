#include "AnimatedObject.hpp"


AnimatedObject::AnimatedObject(void) :
m_texture			(NULL),
m_animationSelected (-1)
{

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
			TextureAnimated::AnimationProperties& anim = this->m_texture->GetAnimation(this->m_animationSelected);
			this->SetFrameCount(anim.m_frameCount);
			this->SetFramerate(anim.m_framerate);
			this->SetLoop(anim.m_loop);
			this->Restart();
		}
		else if (_resetAnimation)
		{
			this->Restart();
		}
		this->OnAnimationChange();
	}
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
			index = anim::AniMath::ModuloPositiveI(index, (int)this->m_texture->GetAnimationCount());
		}
		this->SetAnimation(index, _resetAnimation);
	}
}

bool AnimatedObject::HasAnimation(void)
{
	return this->m_animationSelected != -1;
}

void AnimatedObject::SetTexture(TextureAnimated& _texture, bool _keepAnimation)
{
	std::string* lastAnim = NULL;
	if (_keepAnimation)
	{
		lastAnim = &this->m_texture->GetAnimation(this->m_animationSelected).m_name;
	}
	this->m_texture = &_texture;

	if (_keepAnimation)
	{
		this->SetAnimation(*lastAnim);
	}
	else
	{
		if (!this->m_texture->AnimationExists(this->m_animationSelected))
		{
			this->SetAnimation(0, true);
		}
		else
		{
			this->OnFrameChange();
		}
	}
}

void AnimatedObject::OnAnimationChange(void)
{
	this->OnFrameChange();
}

// AnimatedObject || v1.3