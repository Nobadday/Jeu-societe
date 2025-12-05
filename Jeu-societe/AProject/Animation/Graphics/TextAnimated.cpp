#include "TextAnimated.hpp"


TextAnimated::TextAnimated(void) : TextPlus(),
m_easing (anim::Easing::LINEAR)
{

}
TextAnimated::TextAnimated(const sf::String& _string, const sf::Font& _font, unsigned _characterSize) : TextPlus(_string, _font, _characterSize),
m_easing (anim::Easing::LINEAR)
{

}

void TextAnimated::SetEasing(anim::Easing::Type _easing)
{
	this->m_easing = _easing;
	this->FrameChanged();
}

void TextAnimated::FrameChanged(void)
{
	this->SetStringDisplay(anim::Easing::GetCoefficient(this->GetFrameCoefficient(), this->m_easing));
}

// TextAnimated C++ || v1.1