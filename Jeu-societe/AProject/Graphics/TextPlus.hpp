#pragma once 

#ifndef _INC_TEXTPLUS_HPP
#define _INC_TEXTPLUS_HPP


#include <SFML/Graphics.hpp>



class TextPlus : public sf::Text
{
	private:
		// Unedited string
		std::string m_string;

		// 0 : String Start
		// 1 : String End
		size_t m_textRange[2];

		// Origin in coefficients
		sf::Vector2f m_origin;

		bool m_shouldUpdate;

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
		void setString(const std::string& _string, bool _resetRange);

		const std::string& getString(void);
		// Gets displayed string
		const std::string& GetDisplayedString(void);

		size_t GetDisplayRangeStart(void);
		size_t GetDisplayRangeEnd(void);
};


#endif