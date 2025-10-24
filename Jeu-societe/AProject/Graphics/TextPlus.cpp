#include "TextPlus.hpp"

TextPlus::TextPlus(void) : sf::Text(),
m_string		(""),
m_originCoef	(0,0),
m_shouldFitSize (false)
{
	this->m_characterSize = this->sf::Text::getCharacterSize();

}

void TextPlus::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (this->m_shouldUpdate)
	{

	}
	target.draw((sf::Text)*this, states);
}

void TextPlus::ResetDisplayRange(void)
{
	this->SetDisplayRange(0, this->m_string.size());
}


void TextPlus::SetDisplayRange(size_t _start, size_t _end)
{
	this->m_textRange[0] = _start;
	this->m_textRange[1] = _end;
	
}

void TextPlus::SetDisplayRange(float _coefficient)
{
	this->SetDisplayRange(0, (size_t)((float)this->m_string.size() * _coefficient));
}

void TextPlus::setOrigin(const sf::Vector2f& _origin)
{
	this->m_originCoef = _origin;
}

void TextPlus::setString(const std::string& _string, bool _resetRange)
{
	this->m_string = _string;
}
void TextPlus::setString(const std::string& _string)
{
	this->setString(_string, true);
}



const std::string& TextPlus::getString(void)
{
	return this->m_string;
}
