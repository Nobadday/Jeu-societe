#pragma once

#ifndef _INC_CAMERA_HPP
#define _INC_CAMERA_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>


class Camera
{
	private:
		sf::View m_view;

		sf::Vector2f m_position;
		sf::Vector2f m_size;

		float m_zoom;

		sf::FloatRect m_limits;
		bool m_isLimited;

		mutable bool m_shouldUpdate;

	public:
		Camera(void);
		Camera(const sf::View& _view);
		Camera(const sf::RenderWindow& _renderWindow);
		Camera(Camera& _copy);

		void SetCenter(float _x, float _y);
		void SetCenter(const sf::Vector2f& _center);

		void SetZoom(float _factor);
		
		void SetSize(float _width, float _height);
		void SetSize(const sf::Vector2f& _size);

		void SetRotation(float _angle);

		// Set physical limitations for the camera
		void SetLimitations(const sf::FloatRect& _rect, bool _enableLimitations = true);
		
		// Enables or not limitations
		void SetLimitationCondition(bool _condition);
		void ToggleLimitations(void);

		void SetViewport(const sf::FloatRect& _viewport);

		const sf::Vector2f& GetCenter(void) const;
		const sf::Vector2f& GetSize(void) const;
		float GetZoom(void) const;

		// Returns a rect of the displayed area
		sf::FloatRect GetRenderRect(void);

		const sf::FloatRect& GetLimitations(void) const;
		bool IsLimited(void) const;


		void Reset(const sf::View& _view);
		void Reset(const sf::FloatRect& _rectangle);

		void Move(float _offsetX, float _offsetY);
		void Move(const sf::Vector2f& _offset);
		void Rotate(float _angle);
		void Zoom(float _factor);
		void ZoomAdd(float _value);

		// Returns true if the camera has changed and need to be re-calculated
		// (re-calculated is done when calling GetView())
		bool NeedsUpdate(void);

		const sf::View& GetView(void);
};


#endif
// Camera C++ for SFML 2.6.2 || v.1.0