#ifndef CAMERA_H
#define CAMERA_H

#include "Common.hpp"

class Camera
{
	public:
	Camera(sf::Vector3f _pos);
	~Camera(void);
	static void Update(float _dt);
	static void Draw(sf::RenderWindow* const _renderWindow);
	sf::Vector3f position;
	static std::vector<Camera*>& GetAllCameras();
	Camera& getCamera();
	sf::Vector2f RelatifToScreen(sf::Vector2f _world);
	static void MoveCamera2D(float _dt, Camera& _cam, float _speed);
	static void MoveCamera3D(float _dt, Camera& _cam, float _speed);
private:
	sf::View* view;
	sf::Vector2f size;
};

#endif