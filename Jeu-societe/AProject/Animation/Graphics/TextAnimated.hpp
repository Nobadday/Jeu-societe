#pragma once
#ifndef _INC_TEXTANIMATED_HPP
#define _INC_TEXTANIMATED_HPP

#include "../Animation/AnimationHandler.hpp"
#include "../Animation/AnimationEasing.hpp"
#include "TextPlus.hpp"


class TextAnimated : public TextPlus, public Animation
{
	private:
		anim::Easing::Type m_easing;

	public:
		TextAnimated(void);
		TextAnimated(const sf::String& _string, const sf::Font& _font, unsigned _characterSize = 30u);

		void SetEasing(anim::Easing::Type _easing);
	
	protected:
		virtual void FrameChanged(void);

};




#endif
// TextAnimated C++ || v1.0