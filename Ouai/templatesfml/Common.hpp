#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stdarg.h>
#include <process.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "NewMath.hpp"
#include <string.h>
#include <array>
#include "time.h"
#include <memory>
#include "Binds.hpp"
#include <vector>
#include "SFML/OpenGL.hpp"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 738
#define BPP 32
#define FLT_MAX 9999999

typedef signed short ssi;
typedef unsigned short usi;
typedef struct UIntRect
{
	usi left;
	usi top;
	usi width;
	usi height;
}UIntRect;

#endif