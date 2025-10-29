#pragma once 

#ifndef _INC_TEXTPLUS_HPP
#define _INC_TEXTPLUS_HPP


#include <SFML/Graphics.hpp>


class TransformPlus : public virtual sf::Transformable
{
	private:
		sf::Vector2f m_originCoef;

	public: 

		virtual sf::FloatRect getLocalBounds(void) = 0;

		void setOriginCoef(sf::Vector2f _origin)
		{
			this->m_originCoef = _origin;
		}

		void UpdateOrigin(void)
		{
			sf::Vector2f size = this->getLocalBounds().getSize();
			this->setOrigin(size.x * this->m_originCoef.x, size.y * this->m_originCoef.y);
		}
};


class TextPlus : public sf::Text
{
	private:
		// Unedited string
		std::string m_string;

		// 0 : String Start
		// 1 : String End
		size_t m_textRange[2];

		unsigned m_characterSize;
		float m_characterSizeScale;

		// Origin in coefficients
		sf::Vector2f m_originCoef;

		bool m_shouldFitSize;
		sf::Vector2f m_fitSize;


		mutable bool m_shouldUpdateGeometry;
		mutable bool m_shouldUpdate;

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	public:
		TextPlus(void);

		// Resets the display range back to the original string lenght
		void ResetDisplayRange(void);

		// Set the strings's display range in indexes
		void SetDisplayRange(size_t _start, size_t _end);

		// Set the display range using a coefficient from 0 to _str lenght
		void SetDisplayRange(float _coefficient);

		// Set Origin in coefficients
		void setOrigin(const sf::Vector2f& _origin);

		void setString(const std::string& _string);
		void setString(const std::string& _string, bool _resetRange = true);

		const std::string& getString(void);
		// Gets displayed string
		const std::string& GetDisplayedString(void);

		size_t GetDisplayRangeStart(void);
		size_t GetDisplayRangeEnd(void);
};


#endif