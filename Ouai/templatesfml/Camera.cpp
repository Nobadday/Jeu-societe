#include "Camera.hpp"

std::vector<Camera*> allViews;

Camera::Camera(sf::Vector3f _pos)
{
	position = _pos;
	size = sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT);
	sf::Vector2f center = sf::Vector2f(position.x, position.y);
	view = new sf::View(center, size);
	allViews.push_back(this);
}

Camera::~Camera(void)
{
	delete view;
	view = NULL;
	for (size_t i = 0; i < allViews.size(); i++)
	{
		if (allViews[i] == this)
		{
			allViews.erase(allViews.begin() + i);
			break;
		}
	}
}

void Camera::Update(float _dt)
{
	for(Camera* cam : GetAllCameras())
	{
		cam->view->setCenter(sf::Vector2f(cam->position.x,cam->position.y));
	}
}

void Camera::Draw(sf::RenderWindow* const _renderWindow)
{
	for (Camera* cam : GetAllCameras())
	{
		_renderWindow->setView(*cam->view);
	}
}

std::vector<Camera*>& Camera::GetAllCameras()
{
	return allViews;
}

Camera& Camera::getCamera()
{
	return *this;
}

sf::Vector2f Camera::RelatifToScreen(sf::Vector2f _wrold)
{
	return sf::Vector2f(_wrold.x, _wrold.y) - (size / 2.f);
}

void Camera::MoveCamera2D(float _dt, Camera& _cam, float _speed)
{
	_cam.position.x += GetAxis2D(0).x * _speed * _dt;
	_cam.position.y += GetAxis2D(0).y * _speed * _dt;
}

void Camera::MoveCamera3D(float _dt, Camera& _cam, float _speed)
{
	_cam.position.x += GetAxis3D(0).x * _speed * _dt;
	_cam.position.y += GetAxis3D(0).y * _speed * _dt;
	_cam.position.z += GetAxis3D(0).z * _speed/350 * _dt;
}