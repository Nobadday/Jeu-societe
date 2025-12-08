#pragma once
#ifndef _INC_HEALTHBAR_HPP
#define _INC_HEALTHBAR_HPP


#include <SFML/Graphics.hpp>



class HealthBar : public sf::Drawable, public sf::Transformable
{
	private:
		mutable sf::RectangleShape m_rectShape;

		// 0 : BG
		// 1 : Filler
		const sf::Texture* m_textures[2];
		sf::Color m_colors[2];

		sf::Vector2f m_size;

		float m_fillCoefficient;

		bool m_avoidOverflow;

		bool m_isVertical;


	public:
		HealthBar(void);
		~HealthBar(void);
		

		// Set how far the bar is
		void SetBarCompletion(float _coefficient);

		// Set how far the bar is using virtual health system or something
		void SetBarCompletion(float _value, float _maximum, float _minimum = 0.0f);

		// If _autoVertical is True : Sets it as vertical if the height is higher than the width
		void SetSize(const sf::Vector2f& _size, bool _autoVertical = true);

		// If set to True, the bar will not overflow
		void SetAvoidOverflow(bool _avoidOverflow);

		void SetVertical(bool _vertical);

		// False for BG
		// True for Filler
		void SetTexture(const sf::Texture* _texture, bool _filler, bool _resetColor = true);
		void SetTextureBG(const sf::Texture* _texture, bool _resetColor = true);
		void SetTextureFiller(const sf::Texture* _texture, bool _resetColor = true);


		// False for BG
		// True for Filler
		void SetColor(const sf::Color& _color, bool _filler);
		void SetColorBG(const sf::Color& _color);
		void SetColorFiller(const sf::Color& _color);


		sf::FloatRect GetLocalBounds(void);
		sf::FloatRect GetGlobalBounds(void);

		// Get the coefficient of the completion of the bar
		float GetBarCompletion(void);

		bool IsVertical(void);

	protected:
		void SetRect(const sf::Color& _color, const sf::Texture* _texture, float _fillCoef = 1.0f) const;

		virtual void DrawBG(sf::RenderTarget& _target, sf::RenderStates _states) const;
		virtual void DrawFiller(sf::RenderTarget& _target, sf::RenderStates _states) const;

	private:
		virtual void draw(sf::RenderTarget& _target, sf::RenderStates _states) const;

};



#endif

// HealthBar C++ || v1.1.2