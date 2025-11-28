#pragma once

#ifndef _INC_HEALTHBAR_HPP
#define _INC_HEALTHBAR_HPP


#include <SFML/Graphics.hpp>



class HealthBar : public sf::Drawable, public sf::Transformable
{
	private:
		sf::Texture* m_textureBG;
		sf::Texture* m_textureFiller;

		sf::Color m_colorBG;
		sf::Color m_colorFiller;

		sf::Vector2f m_size;

	protected:
		mutable sf::RectangleShape m_rectShape;

		float m_fillCoefficient;

	protected:
		void SetRect(const sf::Color& _color, const sf::Texture* _texture, float _fillCoef = 1.0f) const;

		virtual void DrawBG(sf::RenderTarget& _target, sf::RenderStates _states) const;
		virtual void DrawFiller(sf::RenderTarget& _target, sf::RenderStates _states) const;

	private:
		virtual void draw(sf::RenderTarget& _target, sf::RenderStates _states) const;


	public:
		HealthBar(void);
		~HealthBar(void);
		

		// Set how far the bar is
		void SetBarCompletion(float _coefficient);

		// Set how far the bar is using virtual health system or something
		void SetBarCompletion(float _value, float _maximum, float _minimum = 0.0f);


		void SetSize(const sf::Vector2f& _size);

		// TODO :
		//void SetTextureBG(const sf::Texture* _texture);

		void SetColorBG(const sf::Color& _color);
		void SetColorFiller(const sf::Color& _color);


		sf::FloatRect GetLocalBounds(void);
		sf::FloatRect GetGlobalBounds(void);

		float GetBarCompletion(void);

};



#endif

// HealthBar C++ || v1.0