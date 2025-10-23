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
		int m_textRange[2];

		// Origin in coefficients
		sf::Vector2f m_origin;

	public:
		TextPlus(void);


		void setString(const std::string& _string);
};


#endif