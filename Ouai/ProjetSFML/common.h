#ifndef COMMON__H
#define COMMON__H

#include <iostream>
#include <time.h>

#include "SFML/Window.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/OpenGL.hpp"


#define SCREEN_SIZE_X 1920
#define SCREEN_SIZE_Y 1080

enum Scene
{
	NONE,
	SCENE_1,
	SCENE_2,
	GAME_OVER
};


#endif