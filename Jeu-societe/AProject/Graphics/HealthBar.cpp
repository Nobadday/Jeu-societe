#include "HealthBar.hpp"

#define DEFAULT_COLOR_BG sf::Color::Red
#define DEFAULT_COLOR_FILLER sf::Color::Green

HealthBar::HealthBar(void) :
	m_textureBG		(NULL),
	m_textureFiller	(NULL),

	m_colorBG		(DEFAULT_COLOR_BG),
	m_colorFiller	(DEFAULT_COLOR_FILLER),

	m_fillCoefficient (1.0f)
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


void HealthBar::SetSize(const sf::Vector2f& _size)
{
	this->m_size = _size;
}

void HealthBar::SetColorBG(const sf::Color& _color)
{
	this->m_colorBG = _color;
}
void HealthBar::SetColorFiller(const sf::Color& _color)
{
	this->m_colorFiller = _color;
}


sf::FloatRect HealthBar::GetLocalBounds(void)
{
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

void HealthBar::SetRect(const sf::Color& _color, const sf::Texture* _texture, float _fillCoef) const
{
	this->m_rectShape.setFillColor(_color);
	this->m_rectShape.setTexture(_texture, false);
	if (_texture != NULL)
	{
		// I have texture
		this->m_rectShape.setSize(this->m_size);
		this->m_rectShape.setTextureRect(sf::IntRect(0, 0, (int)(this->m_size.x * _fillCoef), (int)this->m_size.y));
	}
	else
	{
		// No texture
		this->m_rectShape.setSize(sf::Vector2f(this->m_size.x * _fillCoef, this->m_size.y));
		this->m_rectShape.setTextureRect(sf::IntRect(0, 0, (int)this->m_size.x, (int)this->m_size.y));
		
	}
}

void HealthBar::DrawBG(sf::RenderTarget& _target, sf::RenderStates _states) const
{
	this->SetRect(this->m_colorBG, this->m_textureBG);
	_target.draw(this->m_rectShape, _states);
}
void HealthBar::DrawFiller(sf::RenderTarget& _target, sf::RenderStates _states) const
{
	this->SetRect(this->m_colorFiller, this->m_textureFiller, this->m_fillCoefficient);
	_target.draw(this->m_rectShape, _states);
}


void HealthBar::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
{
	_states.transform.combine(this->getTransform());

	this->DrawBG(_target, _states);
	this->DrawFiller(_target, _states);
}

// HealthBar C++ || v1.0