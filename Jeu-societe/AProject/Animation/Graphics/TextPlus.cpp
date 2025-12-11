#include "TextPlus.hpp"

TextPlus::TextPlus(void) : sf::Text(),
m_thisText			(this),
m_characterSize		(this->sf::Text::getCharacterSize()),
m_outlineThickness	(this->sf::Text::getOutlineThickness()),
m_characterScale	(1.0f),
m_originCoef		(0, 0),
m_string			(""),
m_stringDisplay		(),
m_charactersPerLine (0),
m_alignement		(Alignement::LEFT),
m_shouldUpdate		(false)
{

}
TextPlus::TextPlus(const sf::String& _string, const sf::Font& _font, unsigned _characterSize) : TextPlus()
{
	this->setString(_string);
	this->setFont(_font);
	this->setCharacterSize(_characterSize);
}


void TextPlus::SetStringDisplay(size_t _start, size_t _end)
{
	this->m_stringDisplay[0] = _start;
	this->m_stringDisplay[1] = _end;
	this->m_shouldUpdate = true;
}
void TextPlus::SetStringDisplay(float _coefficient)
{
	this->SetStringDisplay(0, (size_t)((float)this->m_string.getSize() * _coefficient));
}
void TextPlus::ResetStringDisplay(void)
{
	this->SetStringDisplay(0, this->m_string.getSize());
}


void TextPlus::SetCharactersPerLine(unsigned _lenght)
{
	this->m_charactersPerLine = _lenght;
	this->m_shouldUpdate = true;
}

void TextPlus::setOrigin(const sf::Vector2f& _origin)
{
	this->m_originCoef = _origin;
	this->m_shouldUpdate = true;
}

void TextPlus::setFont(const sf::Font& _font)
{
	this->sf::Text::setFont(_font);
	this->m_shouldUpdate = true;
}
void TextPlus::setLineSpacing(float _spacingFactor)
{
	this->sf::Text::setLineSpacing(_spacingFactor);
	this->m_shouldUpdate = true;
}
void TextPlus::setLetterSpacing(float _spacingFactor)
{
	this->sf::Text::setLetterSpacing(_spacingFactor);
	this->m_shouldUpdate = true;
}
void TextPlus::setStyle(sf::Uint32 _style)
{
	this->sf::Text::setStyle(_style);
	this->m_shouldUpdate = true;
}

void TextPlus::setCharacterSize(unsigned int size)
{
	this->m_characterSize = size;
	this->m_shouldUpdate = true;
}
void TextPlus::setOutlineThickness(float thickness)
{
	this->m_outlineThickness = thickness;
	this->m_shouldUpdate = true;
}

void TextPlus::SetOutline(float _thickness, const sf::Color& _color)
{
	this->setOutlineColor(_color);
	this->setOutlineThickness(_thickness);
}


void TextPlus::SetCharacterScale(float _scale)
{
	if (_scale < 0.0f)
	{
		_scale = 0.0f;
	}
	this->m_characterScale = _scale;
	this->m_shouldUpdate = true;
}

void TextPlus::SetAlignement(Alignement _alignement)
{
	this->m_alignement = _alignement;
	this->m_shouldUpdate = true;
}

void TextPlus::setString(const std::string& _string, bool _resetRange)
{
	this->m_string = _string;
	if (_resetRange)
	{
		this->ResetStringDisplay();
	}
	this->m_shouldUpdate = true;
}
void TextPlus::setString(const sf::String& _string)
{
	this->setString(_string, true);
}


const sf::String& TextPlus::getString(void)
{
	return this->m_string;
}
const sf::String& TextPlus::GetDisplayedString(void)
{
	this->UpdateGeometry();
	return this->sf::Text::getString();
}

size_t TextPlus::GetStringDisplay(bool _endRange)
{
	return this->m_stringDisplay[_endRange];
}

sf::FloatRect TextPlus::getLocalBounds(void)
{
	this->UpdateGeometry();
	return this->sf::Text::getLocalBounds();
}
sf::FloatRect TextPlus::getGlobalBounds(void)
{
	this->UpdateGeometry();
	return this->sf::Text::getGlobalBounds();
}

void TextPlus::UpdateGeometry(void) const
{
	if (this->m_shouldUpdate && (this->m_thisText->getFont() != NULL))
	{
		// New sizes
		this->m_thisText->setCharacterSize((unsigned)((float)this->m_characterSize * this->m_characterScale));
		this->m_thisText->setOutlineThickness(this->m_outlineThickness * this->m_characterScale);


		// New string
		std::string newString = this->m_string.substring(this->m_stringDisplay[0], this->m_stringDisplay[1]);

		if (newString.empty())
		{
			// String is empty, no need to do all of this stuff
			this->m_thisText->setString("");
			this->m_shouldUpdate = false;
			return;
		}

		// Character per line restrictions, adds \n to the string
		if (this->m_charactersPerLine > 0)
		{
			int lastSpace = -1;
			int lineLen = 0;
			for (int i = 0; i < newString.size(); i++)
			{
				switch (newString[i])
				{
					case '\n':
						lineLen = 0;
						lastSpace = -1;
						break;

					case ' ':
						lastSpace = i;
						[[fallthrough]];
					default:
						lineLen++;
						break;
				}
				if (lineLen > (int)this->m_charactersPerLine)
				{
					if (lastSpace == -1)
					{
						newString.insert(i, 1, '\n');
						lineLen = 0;
					}
					else
					{
						newString.replace(lastSpace, 1, 1, '\n');
						lineLen = i - lastSpace;
						lastSpace = -1;
					}
				}
			}
		}

		// Paragraph alignement only if alignement ain't LEFT
		if (this->m_alignement != Alignement::LEFT)
		{

			// 1) Get the max amount of characters/line
			int lineLenMax;
			if (this->m_charactersPerLine == 0)
			{
				// Must iterate through string to get the max char/line
				lineLenMax = 0;
				int tempCharCounter = 0;
				for (int i = 0; i < newString.size(); i++)
				{
					if ((newString[i] == '\n') || (i + 1 >= newString.size()))
					{
						if (tempCharCounter > lineLenMax)
						{
							lineLenMax = tempCharCounter;
						}
						tempCharCounter = 0;
					}
					else
					{
						tempCharCounter++;
					}
				}
			}
			else
			{
				lineLenMax = this->m_charactersPerLine;
			}

			// 2) Shift the strings to align them
			int paraLineLen = 0;

			int tempCharCounter = 0;
			for (int i = 0; i < newString.size(); i++)
			{
				if ((newString[i] == '\n') || (i + 1 >= newString.size()))
				{
					int missingSpace = (lineLenMax - tempCharCounter);

					if (missingSpace > 0)
					{
						int shift = 0;
						switch (this->m_alignement)
						{
							case Alignement::RIGHT:
								shift = missingSpace;
								break;

							case Alignement::CENTER:
								shift = missingSpace / 2;
								break;

							case Alignement::LEFT:
							default:
								break;
						}
						if (shift > 0)
						{
							newString.insert(i - tempCharCounter, shift, ' ');
							i += shift;
						}
					}

					tempCharCounter = 0;
				}
				else
				{
					tempCharCounter++;
				}
			}

			//End Alignement
		}


		// Sets the final string
		this->m_thisText->setString(newString);


		// Set the origin with coefficients
		
		sf::FloatRect localBounds = this->m_thisText->getLocalBounds();

		this->m_thisText->setOrigin(localBounds.width * this->m_originCoef.x,
									localBounds.height * this->m_originCoef.y);

		this->m_shouldUpdate = false;
	}
}

void TextPlus::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	this->UpdateGeometry();
	target.draw((sf::Text)*this, states);
}


// TextPlus C++ || v1.0