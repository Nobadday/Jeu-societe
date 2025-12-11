#pragma once 
#ifndef _INC_TEXTPLUS_HPP
#define _INC_TEXTPLUS_HPP


#include <SFML/Graphics.hpp>
#include <stdio.h>

class TextPlus : public sf::Text
{
	public:
		enum Alignement
		{
			LEFT,
			CENTER,
			RIGHT
		};

	private:
		// Mutable pointer of base class
		mutable sf::Text* m_thisText;

		unsigned m_characterSize;
		float m_outlineThickness;
		float m_characterScale;

		// Origin in coefficients
		sf::Vector2f m_originCoef;

		// Unedited string
		sf::String m_string;

		// 0 : String Start
		// 1 : String End
		size_t m_stringDisplay[2];

		unsigned m_charactersPerLine;
		Alignement m_alignement;

		mutable bool m_shouldUpdate;

	public:
		TextPlus(void);
		TextPlus(const sf::String& _string, const sf::Font& _font, unsigned _characterSize = 30u);

		
		// Set the strings's display range in indexes
		void SetStringDisplay(size_t _start, size_t _end);
		// Set the string display using a coefficient from 0 to _str lenght
		void SetStringDisplay(float _coefficient);
		// Resets the string display back to the original string lenght
		void ResetStringDisplay(void);

		// Set the amount of characters displayed per line
		// 0 to disable
		void SetCharactersPerLine(unsigned _lenght);


		// Set Origin in coefficients
		// (0, 0) = top left corner
		void setOrigin(const sf::Vector2f& _origin);

		// Wrapper functions that just sets shouldupdate

		void setFont(const sf::Font& _font);
		void setLineSpacing(float _spacingFactor);
		void setLetterSpacing(float _spacingFactor);
		void setStyle(sf::Uint32 _style);
		// End Wrapper functions

		void setCharacterSize(unsigned int size);
		void setOutlineThickness(float thickness);

		void SetOutline(float _thickness, const sf::Color& _color);

		// Sets the scale of the character size to be able to scale your text without losing quality
		// CharacterSize * scale
		// (Also scales Outline thickness)
		// Negative values are ignored, to reverse your text, use setScale()
		void SetCharacterScale(float _scale);

		// Set the paragraph alignement
		void SetAlignement(Alignement _alignement);

		void setString(const sf::String& _string);
		void setString(const std::string& _string, bool _resetRange);

		// Gets the unedited string
		const sf::String& getString(void);
		// Gets displayed string
		const sf::String& GetDisplayedString(void);


		size_t GetStringDisplay(bool _endRange);

		sf::FloatRect getLocalBounds(void);
		sf::FloatRect getGlobalBounds(void);
	
	private:
		void UpdateGeometry(void) const;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};


#endif

// TextPlus C++ || v1.0
// TODO :
// Getters that calls UpdateGeo