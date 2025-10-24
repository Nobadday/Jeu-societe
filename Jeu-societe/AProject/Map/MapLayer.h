#ifndef MAPLAYER_H
#define MAPLAYER_H

#include "../Common.hpp"

#include "MapObject.h"

class TilesetManager;
class Camera;

class MapLayer
{
private:
    int m_id;                           // ID du layer
    std::string m_name;                 // Nom du layer
    std::string m_type;                 // Type du layer (objectgroup, tilelayer, etc.)
    std::vector<MapObject> m_objects;   // Liste des objets du layer
    bool m_visible;                     // Visibilité du layer
    float m_opacity;                    // Opacité du layer (0.0 à 1.0)
    float m_parallaxX;                  // Facteur de parallaxe X
    float m_parallaxY;                  // Facteur de parallaxe Y
    sf::Color m_tintColor;              // Couleur de teinte

public:
    // Constructeur
    MapLayer();
    MapLayer(int id, const std::string& name, const std::string& type);

    // Getters
    int GetId() const { return m_id; }
    const std::string& GetName() const { return m_name; }
    const std::string& GetType() const { return m_type; }
    const std::vector<MapObject>& GetObjects() const { return m_objects; }
    bool IsVisible() const { return m_visible; }
    float GetOpacity() const { return m_opacity; }
    float GetParallaxX() const { return m_parallaxX; }
    float GetParallaxY() const { return m_parallaxY; }
    const sf::Color& GetTintColor() const { return m_tintColor; }

    // Setters
    void SetId(int id) { m_id = id; }
    void SetName(const std::string& name) { m_name = name; }
    void SetType(const std::string& type) { m_type = type; }
    void SetVisible(bool visible) { m_visible = visible; }
    void SetOpacity(float opacity) { m_opacity = opacity; }
    void SetParallaxX(float parallaxX) { m_parallaxX = parallaxX; }
    void SetParallaxY(float parallaxY) { m_parallaxY = parallaxY; }
    void SetTintColor(const sf::Color& color) { m_tintColor = color; }

    // Gestion des objets
    void AddObject(const MapObject& object);
    MapObject& GetObject(size_t index);
    const MapObject& GetObject(size_t index) const;
    MapObject& GetObject(std::string _name);
    size_t GetObjectCount() const { return m_objects.size(); }
    void ClearObjects() { m_objects.clear(); }

    // Rendu
    void Draw(sf::RenderTarget& target, TilesetManager& tilesetManager, const sf::Vector2f& camera) const;
};
#endif // MAPLAYER_H