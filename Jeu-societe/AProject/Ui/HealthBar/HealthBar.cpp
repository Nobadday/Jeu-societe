#include "HealthBar.hpp"


#define DEFAULT_COLOR_BG sf::Color::Red
#define DEFAULT_COLOR_FILLER sf::Color::Green



HealthBar::HealthBar(void) :
	m_rectShape		  (),

	m_textures		  {NULL, NULL},
	m_colors		  {DEFAULT_COLOR_BG, DEFAULT_COLOR_FILLER},

	m_size			  (0,0),
	m_fillCoefficient (1.0f),

	m_avoidOverflow   (false),
	m_isVertical	  (false)
{

}
HealthBar::~HealthBar(void)
{

}

void HealthBar::SetBarCompletion(float _coefficient)
{
	this->m_fillCoefficient = _coefficient;
}
void HealthBar::SetBarCompletion(float _value, float _maximum, float _minimum)
{
	this->SetBarCompletion((_value - _minimum) / (_maximum - _minimum));
}


void HealthBar::SetSize(const sf::Vector2f& _size, bool _autoVertical)
{
	this->m_size = _size;
	if (_autoVertical)
	{
		this->m_isVertical = (this->m_size.y > this->m_size.x);
	}
}

void HealthBar::SetAvoidOverflow(bool _avoidOverflow)
{
	this->m_avoidOverflow = _avoidOverflow;
}

void HealthBar::SetVertical(bool _vertical)
{
	this->m_isVertical = _vertical;
}


void HealthBar::SetTexture(const sf::Texture* _texture, bool _filler, bool _resetColor)
{
	this->m_textures[_filler] = _texture;
	if (_resetColor)
	{
		this->m_colors[_filler] = sf::Color::White;
	}
}
void HealthBar::SetTextureBG(const sf::Texture* _texture, bool _resetColor)
{
	this->SetTexture(_texture, false, _resetColor);
}
void HealthBar::SetTextureFiller(const sf::Texture* _texture, bool _resetColor)
{
	this->SetTexture(_texture, true, _resetColor);
}

void HealthBar::SetColor(const sf::Color& _color, bool _filler)
{
	this->m_colors[_filler] = _color;
}
void HealthBar::SetColorBG(const sf::Color& _color)
{
	this->SetColor(_color, false);
}
void HealthBar::SetColorFiller(const sf::Color& _color)
{
	this->SetColor(_color, true);
}


sf::FloatRect HealthBar::GetLocalBounds(void)
{
	this->m_rectShape.setSize(this->m_size);
	return this->m_rectShape.getLocalBounds();
}
sf::FloatRect HealthBar::GetGlobalBounds(void)
{
	return this->getTransform().transformRect(this->GetLocalBounds());
}

float HealthBar::GetBarCompletion(void)
{
	return this->m_fillCoefficient;
}

bool HealthBar::IsVertical(void)
{
	return this->m_isVertical;
}

void HealthBar::SetRect(const sf::Color& _color, const sf::Texture* _texture, float _fillCoef) const
{
	if (this->m_avoidOverflow)
	{
		if (_fillCoef > 1.0f)
		{
			_fillCoef = 1.0f;
		}
		else if (_fillCoef < 0.0f)
		{
			_fillCoef = 0.0f;
		}
	}

	this->m_rectShape.setFillColor(_color);
	this->m_rectShape.setTexture(_texture, false);

	sf::Vector2f sizeCoeffed;
	if (this->m_isVertical)
	{
		sizeCoeffed.x = this->m_size.x;
		sizeCoeffed.y = this->m_size.y * _fillCoef;
	}
	else
	{
		sizeCoeffed.x = this->m_size.x * _fillCoef;
		sizeCoeffed.y = this->m_size.y;
	}

	if (_texture != NULL)
	{
		// I have texture
		sf::Vector2u tSize = _texture->getSize();
		this->m_rectShape.setSize(this->m_size);
		this->m_rectShape.setTextureRect(sf::IntRect(0, 0, (int)tSize.x * _fillCoef, (int)tSize.y));
	}
	else
	{
		// No texture
		this->m_rectShape.setSize(sizeCoeffed);
		this->m_rectShape.setTextureRect(sf::IntRect(0, 0, (int)this->m_size.x, (int)this->m_size.y));
	}
}


void HealthBar::DrawBG(sf::RenderTarget& _target, sf::RenderStates _states) const
{
	this->SetRect(this->m_colors[0], this->m_textures[0]);
	_target.draw(this->m_rectShape, _states);
}
void HealthBar::DrawFiller(sf::RenderTarget& _target, sf::RenderStates _states) const
{
	this->SetRect(this->m_colors[1], this->m_textures[1], this->m_fillCoefficient);
	_target.draw(this->m_rectShape, _states);
}


void HealthBar::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
{
	_states.transform.combine(this->getTransform());

	this->DrawBG(_target, _states);
	this->DrawFiller(_target, _states);
}

// HealthBar C++ || v1.1.2