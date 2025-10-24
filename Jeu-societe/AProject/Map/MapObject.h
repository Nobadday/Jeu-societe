#ifndef MAPOBJECT_H
#define MAPOBJECT_H

#include "../Common.hpp"

class MapObject
{
private:
    int m_gid;                    // Global ID de la tuile
    sf::Vector2f m_position;      // Position (x, y)
    sf::Vector2f m_size;          // Taille (width, height)
    std::string m_name;           // Nom de l'objet
    std::string m_type;           // Type de l'objet
    float m_rotation;             // Rotation en degrés
    bool m_visible;               // Visibilité de l'objet

public:
    // Constructeurs
    MapObject();
    MapObject(int gid, const sf::Vector2f& position, const sf::Vector2f& size);
    MapObject(int gid, float x, float y, float width, float height);

    // Getters
    int getGid() const { return m_gid; }
    const sf::Vector2f& getPosition() const { return m_position; }
    const sf::Vector2f& getSize() const { return m_size; }
    const std::string& getName() const { return m_name; }
    const std::string& getType() const { return m_type; }
    float getRotation() const { return m_rotation; }
    bool isVisible() const { return m_visible; }
    
    float getX() const { return m_position.x; }
    float getY() const { return m_position.y; }
    float getWidth() const { return m_size.x; }
    float getHeight() const { return m_size.y; }

    // Setters
    void setGid(int gid) { m_gid = gid; }
    void setPosition(const sf::Vector2f& position) { m_position = position; }
    void setPosition(float x, float y) { m_position = sf::Vector2f(x, y); }
    void setSize(const sf::Vector2f& size) { m_size = size; }
    void setSize(float width, float height) { m_size = sf::Vector2f(width, height); }
    void setName(const std::string& name) { m_name = name; }
    void setType(const std::string& type) { m_type = type; }
    void setRotation(float rotation) { m_rotation = rotation; }
    void setVisible(bool visible) { m_visible = visible; }

    // Méthodes utilitaires
    sf::FloatRect getBounds() const;
    bool contains(const sf::Vector2f& point) const;
};
#endif // MAPOBJECT_H