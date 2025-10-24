#include "Tiled.h"
#include <sstream>

void Tiled::InitTiled(const char* _Map)
{
    if (config.LoadFromFile(_Map))
    {
        // Lire les propriétés de la carte
        int compressionLevel = config.GetValue<int>("compressionlevel", -1);
        int mapHeight = config.GetValue<int>("height", 0);
        int mapWidth = config.GetValue<int>("width", 0);
        int tileWidth = config.GetValue<int>("tilewidth", 32);
        int tileHeight = config.GetValue<int>("tileheight", 32);
        std::string orientation = config.GetValue<std::string>("orientation", "orthogonal");
        bool infinite = config.GetValue<bool>("infinite", false);

        const auto& jsonData = config.GetData();

        // Charger les tilesets en premier
        if (jsonData.contains("tilesets") && jsonData["tilesets"].is_array())
        {
            std::cout << "=== CHARGEMENT DES TILESETS ===" << std::endl;
            const auto& tilesets = jsonData["tilesets"];

            for (size_t i = 0; i < tilesets.size(); ++i)
            {
                const auto& tilesetJson = tilesets[i];

                Tileset tileset;
                tileset.SetName(tilesetJson.value("name", "Sans nom"));
                tileset.SetFirstGid(tilesetJson.value("firstgid", 0));
                tileset.SetTileCount(tilesetJson.value("tilecount", 0));
                tileset.SetTileWidth(tilesetJson.value("tilewidth", 0));
                tileset.SetTileHeight(tilesetJson.value("tileheight", 0));
                tileset.SetColumns(tilesetJson.value("columns", 0));
                tileset.SetMargin(tilesetJson.value("margin", 0));
                tileset.SetSpacing(tilesetJson.value("spacing", 0));
				
                std::cout << "Chargement du tileset: " << tileset.GetName() << std::endl;
				std::cout << "  First GID: " << tileset.GetFirstGid() << std::endl;
				std::cout << "  Tile Count: " << tileset.GetTileCount() << std::endl;
				std::cout << "  Tile Size: " << tileset.GetTileWidth() << "x" << tileset.GetTileHeight() << std::endl;


                // Charger les tuiles individuelles
                if (tilesetJson.contains("tiles") && tilesetJson["tiles"].is_array())
                {
                    const auto& tiles = tilesetJson["tiles"];
                    for (const auto& tile : tiles)
                    {
                        int id = tile.value("id", 0);
                        std::string imagePath = tile.value("image", "");
                        int imageWidth = tile.value("imagewidth", 0);
                        int imageHeight = tile.value("imageheight", 0);

                        tileset.AddTile(id, imagePath, imageWidth, imageHeight);
                    }
                }

                // Ajouter le tileset au gestionnaire
                tilesetManager.AddTileset(tileset);
                //std::cout << std::endl;
            }

            // Charger toutes les textures
            std::cout << "Chargement des textures..." << std::endl;
            if (tilesetManager.LoadAllTextures("Assets/Map/"))
            {
                std::cout << "Toutes les textures ont été chargées avec succès!" << std::endl;
            }
            else
            {
                std::cerr << "Certaines textures n'ont pas pu être chargées." << std::endl;
            }
            std::cout << std::endl;
        }

        // Accéder au tableau des layers
        if (jsonData.contains("layers") && jsonData["layers"].is_array())
        {
            //std::cout << "=== LAYERS ===" << std::endl;
            const auto& layers = jsonData["layers"];

            for (size_t i = 0; i < layers.size(); ++i)
            {
                const auto& layer = layers[i];

                // Créer un nouveau layer
                MapLayer mapLayer;
                mapLayer.SetId(layer.value("id", 0));
                mapLayer.SetName(layer.value("name", "Sans nom"));
                mapLayer.SetType(layer.value("type", ""));
                mapLayer.SetVisible(layer.value("visible", true));
                mapLayer.SetOpacity(layer.value("opacity", 1.0f));
                mapLayer.SetParallaxX(layer.value("parallaxx", 1.0f));
                mapLayer.SetParallaxY(layer.value("parallaxy", 1.0f));

                std::cout << "Layer " << i << ": " << mapLayer.GetName()
                          << " (Type: " << mapLayer.GetType() << ")"
                          << ", Visible: " << (mapLayer.IsVisible() ? "Oui" : "Non")
                          << ", Opacité: " << mapLayer.GetOpacity()
                          << ", Parallaxe: (" << mapLayer.GetParallaxX() << ", " << mapLayer.GetParallaxY() << ")"
					<< std::endl;

                // Lire les objets dans chaque layer
                if (layer.contains("objects") && layer["objects"].is_array())
                {
                    const auto& objects = layer["objects"];
                    std::cout << "  Nombre d'objets: " << objects.size() << std::endl;

                    for (size_t j = 0; j < objects.size(); ++j)
                    {
                        const auto& obj = objects[j];

                        // Créer un nouvel objet
                        MapObject mapObject;
                        int gid = obj.value("gid", 0);
                        mapObject.SetGid(gid);
                        mapObject.SetPosition(obj.value("x", 0.0f), obj.value("y", 0.0f));
                        mapObject.SetSize(obj.value("width", 0.0f), obj.value("height", 0.0f));
                        mapObject.SetName(obj.value("name", ""));
                        mapObject.SetType(obj.value("type", ""));
                        mapObject.SetRotation(obj.value("rotation", 0.0f));
                        mapObject.SetVisible(obj.value("visible", true));

                        // Ajouter l'objet au layer
                        mapLayer.AddObject(mapObject);

                        // Trouver le tileset correspondant
                        const Tileset* tileset = tilesetManager.GetTilesetByGid(gid);
                        std::string tilesetName = tileset ? tileset->GetName() : "Aucun";

                        std::cout << "    Objet " << j << " : " << mapObject.GetName() << " : GID=" << gid
                                  << " (Tileset: " << tilesetName << ")"
                                  << ", Pos=(" << mapObject.GetX() << "," << mapObject.GetY() << ")"
                                  << ", Taille=(" << mapObject.GetWidth() << "x" << mapObject.GetHeight() << ")" << std::endl;
                    }
                }

                // Ajouter le layer à la liste
                mapLayers.push_back(mapLayer);
                std::cout << std::endl;
            }
        }

        std::cout << "\n=== RÉCAPITULATIF ===" << std::endl;
        std::cout << "Nombre total de tilesets: " << tilesetManager.GetTilesetCount() << std::endl;
        std::cout << "Nombre total de layers: " << mapLayers.size() << std::endl;
        for (const auto& layer : mapLayers)
        {
            std::cout << "  - " << layer.GetName() << ": " << layer.GetObjectCount() << " objets" << std::endl;
        }
        std::cout << "\n=== CONTRÔLES ===" << std::endl;
        std::cout << "Flèches directionnelles : Déplacer la caméra" << std::endl;
        std::cout << "Échap : Quitter" << std::endl;
        std::cout << std::endl;
    }
}

std::vector<MapLayer>& Tiled::GetMapLayers()
{
	return mapLayers;
}

MapLayer& Tiled::GetMapLayer(std::string _name)
{
    for (int i = 0; i < mapLayers.size() ; i++)
    {
        if (_name == mapLayers[i].GetName())
        {
            return mapLayers[i];
        }
    }
    return mapLayers[0];
}

void Tiled::DrawMapLayers(sf::RenderWindow& _window, const sf::Vector2f& _camera)
{
      for (const auto& layer : mapLayers)
        {
            layer.Draw(_window, tilesetManager, _camera);
        }
}
