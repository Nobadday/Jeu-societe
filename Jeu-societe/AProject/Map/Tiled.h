#ifndef TILED_H
#define TILED_H

#include "../Common.hpp"

#include "MapObject.h"
#include "MapLayer.h"
#include "Tileset.h"

class Tiled
{
	private:
		std::vector<MapLayer> mapLayers;
		TilesetManager tilesetManager;
		JsonReader config;
	public:
		void InitTiled(const char* _Map);
		std::vector<MapLayer>& GetMapLayers();
		MapLayer& GetMapLayer(std::string _name);
		void DrawMapLayers(sf::RenderWindow& _window, const sf::Vector2f& _camera);
		void DrawMapLayers(sf::RenderWindow& _window, const sf::Vector2f& _camera, std::string _name);
		void DrawMapLayers(sf::RenderWindow& _window, const sf::Vector2f& _camera, const char* _name);
};


#endif // TILED_H
