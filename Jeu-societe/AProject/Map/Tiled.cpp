#include "Tiled.h"

void Tiled::InitTiled(const char* _Map)
{
    if (config.loadFromFile(_Map))
    {
        // Lire les propriétés de la carte
        int compressionLevel = config.getValue<int>("compressionlevel", -1);
        int mapHeight = config.getValue<int>("height", 0);
        int mapWidth = config.getValue<int>("width", 0);
        int tileWidth = config.getValue<int>("tilewidth", 32);
        int tileHeight = config.getValue<int>("tileheight", 32);
        std::string orientation = config.getValue<std::string>("orientation", "orthogonal");
        bool infinite = config.getValue<bool>("infinite", false);

        const auto& jsonData = config.getData();

        // Charger les tilesets en premier
        if (jsonData.contains("tilesets") && jsonData["tilesets"].is_array())
        {
            // std::cout << "=== CHARGEMENT DES TILESETS ===" << std::endl;
            const auto& tilesets = jsonData["tilesets"];

            for (size_t i = 0; i < tilesets.size(); ++i)
            {
                const auto& tilesetJson = tilesets[i];

                Tileset tileset;
                tileset.setName(tilesetJson.value("name", "Sans nom"));
                tileset.setFirstGid(tilesetJson.value("firstgid", 0));
                tileset.setTileCount(tilesetJson.value("tilecount", 0));
                tileset.setTileWidth(tilesetJson.value("tilewidth", 0));
                tileset.setTileHeight(tilesetJson.value("tileheight", 0));
                tileset.setColumns(tilesetJson.value("columns", 0));
                tileset.setMargin(tilesetJson.value("margin", 0));
                tileset.setSpacing(tilesetJson.value("spacing", 0));

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

                        tileset.addTile(id, imagePath, imageWidth, imageHeight);
                    }
                }

                // Ajouter le tileset au gestionnaire
                tilesetManager.addTileset(tileset);
                //std::cout << std::endl;
            }

            // Charger toutes les textures
            std::cout << "Chargement des textures..." << std::endl;
            if (tilesetManager.loadAllTextures("Assets/Map/"))
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

                // Lire les objets dans chaque layer
                if (layer.contains("objects") && layer["objects"].is_array())
                {
                    const auto& objects = layer["objects"];
                    // std::cout << "  Nombre d'objets: " << objects.size() << std::endl;

                    for (size_t j = 0; j < objects.size(); ++j)
                    {
                        const auto& obj = objects[j];

                        // Créer un nouvel objet
                        MapObject mapObject;
                        int gid = obj.value("gid", 0);
                        mapObject.setGid(gid);
                        mapObject.setPosition(obj.value("x", 0.0f), obj.value("y", 0.0f));
                        mapObject.setSize(obj.value("width", 0.0f), obj.value("height", 0.0f));
                        mapObject.setName(obj.value("name", ""));
                        mapObject.setType(obj.value("type", ""));
                        mapObject.setRotation(obj.value("rotation", 0.0f));
                        mapObject.setVisible(obj.value("visible", true));

                        // Ajouter l'objet au layer
                        mapLayer.AddObject(mapObject);

                        // Trouver le tileset correspondant
                        const Tileset* tileset = tilesetManager.getTilesetByGid(gid);
                        std::string tilesetName = tileset ? tileset->getName() : "Aucun";

                        /*std::cout << "    Objet " << j << ": GID=" << gid
                                  << " (Tileset: " << tilesetName << ")"
                                  << ", Pos=(" << mapObject.getX() << "," << mapObject.getY() << ")"
                                  << ", Taille=(" << mapObject.getWidth() << "x" << mapObject.getHeight() << ")" << std::endl;*/
                    }
                }

                // Ajouter le layer à la liste
                mapLayers.push_back(mapLayer);
                std::cout << std::endl;
            }
        }

        //std::cout << "\n=== RÉCAPITULATIF ===" << std::endl;
        //std::cout << "Nombre total de tilesets: " << tilesetManager.getTilesetCount() << std::endl;
        //std::cout << "Nombre total de layers: " << mapLayers.size() << std::endl;
        //for (const auto& layer : mapLayers)
        //{
        //    std::cout << "  - " << layer.getName() << ": " << layer.getObjectCount() << " objets" << std::endl;
        //}
        //std::cout << "\n=== CONTRÔLES ===" << std::endl;
        //std::cout << "Flèches directionnelles : Déplacer la caméra" << std::endl;
        //std::cout << "Échap : Quitter" << std::endl;
        //std::cout << std::endl;
    }
}

void Tiled::DrawMapLayers(sf::RenderWindow& _window, const sf::Vector2f& _camera)
{
      for (const auto& layer : mapLayers)
        {
            layer.Draw(_window, tilesetManager, _camera);
        }
}
