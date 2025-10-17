#include "Camera.hpp"

#define CAM_MIN_ZOOM 0.01f


Camera::Camera(void) : 
m_position		(0, 0),
m_size			(1000,1000),
m_zoom			(1),
m_limits		(0,0,0,0),
m_isLimited		(false),
m_shouldUpdate	(true)
{
}
Camera::Camera(const sf::View& _view) : Camera()
{
	m_view = _view;
	m_position = m_view.getCenter();
	m_size = m_view.getSize();
	m_limits = { m_position.x - m_size.x/2.0f, m_position.y - m_size.y / 2.0f, m_size.x, m_size.y };
}
Camera::Camera(const sf::RenderWindow& _renderWindow) : Camera(_renderWindow.getView())
{
}
Camera::Camera(Camera& _copy)
{
	this->m_position = _copy.m_position;
	this->m_size = _copy.m_size;
	this->m_zoom = _copy.m_zoom;
	this->m_limits = _copy.m_limits;
	this->m_isLimited = _copy.m_isLimited;
	this->m_shouldUpdate = _copy.m_shouldUpdate;
}


void Camera::SetCenter(float _x, float _y)
{
	this->m_position.x = _x;
	this->m_position.y = _y;
	this->m_shouldUpdate = true;
}
void Camera::SetCenter(const sf::Vector2f& _center)
{
	this->SetCenter(_center.x, _center.y);
}


void Camera::SetZoom(float _factor)
{
	if (_factor < CAM_MIN_ZOOM)
	{
		_factor = CAM_MIN_ZOOM;
	}
	this->m_zoom = _factor;
	this->m_shouldUpdate = true;
}


void Camera::SetSize(float _width, float _height)
{
	this->m_size.x = _width;
	this->m_size.y = _height;
	this->m_shouldUpdate = true;
}
void Camera::SetSize(const sf::Vector2f& _size)
{
	this->SetSize(_size.x, _size.y);
}

void Camera::SetRotation(float _angle)
{
	this->m_view.setRotation(_angle);
}

void Camera::SetLimitations(const sf::FloatRect& _rect, bool _enableLimitations)
{
	this->m_limits = _rect;
	if (_enableLimitations)
	{
		this->m_isLimited = true;
	}
	this->m_shouldUpdate = true;
}

void Camera::SetLimitationCondition(bool _condition)
{
	this->m_isLimited = _condition;
	this->m_shouldUpdate = true;
}
void Camera::ToggleLimitations(void)
{
	this->SetLimitationCondition(!this->m_isLimited);
}

void Camera::SetViewport(const sf::FloatRect& _viewport)
{
	this->m_view.setViewport(_viewport);
}

const sf::Vector2f& Camera::GetCenter(void) const
{
	return this->m_position;
}

const sf::Vector2f& Camera::GetSize(void) const
{
	return this->m_size;
}

float Camera::GetZoom(void) const
{
	return this->m_zoom;
}

sf::FloatRect Camera::GetRenderRect(void)
{
	const sf::View& view = this->GetView();
	const sf::Vector2f& size = view.getSize();
	const sf::Vector2f& position = view.getCenter();
	return sf::FloatRect(position.x - (size.x / 2.0f)	, position.y - (size.y / 2.0f),
						 size.x							, size.y);
}

const sf::FloatRect& Camera::GetLimitations(void) const
{
	return this->m_limits;
}

bool Camera::IsLimited(void) const
{
	return this->m_isLimited;
}

void Camera::Reset(const sf::View& _view)
{
	this->m_view = _view;
	this->m_position = _view.getCenter();
	this->m_size = _view.getSize();
	m_limits = { m_position.x - m_size.x / 2.0f, m_position.y - m_size.y / 2.0f, m_size.x, m_size.y };

	this->m_shouldUpdate = true;
}
void Camera::Reset(const sf::FloatRect& _rectangle)
{
	this->m_view.reset(_rectangle);
	this->m_position = this->m_view.getCenter();
	this->m_size = this->m_view.getSize();
	this->m_shouldUpdate = true;
}

void Camera::Move(float _offsetX, float _offsetY)
{
	this->SetCenter(this->m_position.x + _offsetX, this->m_position.y + _offsetY);
}

void Camera::Move(const sf::Vector2f& _offset)
{
	this->Move(_offset.x, _offset.y);
}

void Camera::Rotate(float _angle)
{
	this->m_view.rotate(_angle);
	this->m_shouldUpdate = true;
}

void Camera::Zoom(float _factor)
{
	this->SetZoom(this->m_zoom * _factor);
}
void Camera::ZoomAdd(float _value)
{
	this->SetZoom(this->m_zoom + _value);
}


bool Camera::NeedsUpdate(void)
{
	return this->m_shouldUpdate;
}

const sf::View& Camera::GetView(void)
{
	if (this->m_shouldUpdate)
	{
		sf::Vector2f targetSize(this->m_size.x * this->m_zoom, this->m_size.y * this->m_zoom);
		
		if (this->m_isLimited)
		{
			// Fit Size
			float widthRatio = targetSize.x / this->m_limits.width;
			float heightRatio = targetSize.x / this->m_limits.width;
			if (widthRatio > 1.0f || heightRatio > 1.0f)
			{
				if (widthRatio > heightRatio)
				{
					targetSize.x /= widthRatio;
					targetSize.y /= widthRatio;
				}
				else
				{
					targetSize.x /= heightRatio;
					targetSize.y /= heightRatio;
				}
			}
			
			// Fit Position
			sf::Vector2f posCorner(this->m_position.x - (targetSize.x / 2.0f), this->m_position.y - (targetSize.y / 2.0f));

			if (posCorner.x < this->m_limits.left)
			{
				posCorner.x += this->m_limits.left - posCorner.x;
			}
			else if (posCorner.x + targetSize.x > this->m_limits.left + this->m_limits.width)
			{
				posCorner.x += (this->m_limits.left + this->m_limits.width) - (posCorner.x + targetSize.x);
			}

			if (posCorner.y < this->m_limits.top)
			{
				posCorner.y += this->m_limits.top - posCorner.y;
			}
			else if (posCorner.y + targetSize.y > this->m_limits.top + this->m_limits.height)
			{
				posCorner.y += (this->m_limits.top + this->m_limits.height) - (posCorner.y + targetSize.y);
			}

			this->m_view.setCenter(posCorner.x + (targetSize.x / 2.0f), posCorner.y + (targetSize.y / 2.0f));
		}
		else
		{
			this->m_view.setCenter(this->m_position);
		}
		this->m_view.setSize(targetSize);
		
		this->m_shouldUpdate = false;
	}
	return this->m_view;
}

// Camera C++ for SFML 2.6.2 || v.1.0