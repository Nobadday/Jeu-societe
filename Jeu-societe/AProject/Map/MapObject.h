#ifndef MAPOBJECT_H
#define MAPOBJECT_H

#include "../Common.hpp"

class MapObjectProperty
{
private:
    std::string m_name;           // Nom de l'objet
    std::string m_type;           // Type de l'objet
    std::string m_stringValue;    // 2eme Type de l'objet
    int m_value;                  //value de l'objet int

public:
    // Constructeurs
    MapObjectProperty();

    // Getters
    const std::string& GetName() const { return m_name; }
    const std::string& GetType() const { return m_type; }
    const std::string& GetStringValue() const { return m_stringValue; }
    const int& GetValue() const { return m_value; }


    // Setters
    void SetName(const std::string& name) { m_name = name; }
    void SetType(const std::string& type) { m_type = type; }
    void SetValue(const std::string& value) { m_stringValue = value; }
    void SetValue(const int& value) { m_value = value; }
};

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
	std::vector<MapObjectProperty> m_properties; // Propriétés personnalisées

public:
    // Constructeurs
    MapObject();
    MapObject(int gid, const sf::Vector2f& position, const sf::Vector2f& size);
    MapObject(int gid, float x, float y, float width, float height);

    // Getters
    int GetGid() const { return m_gid; }
    const sf::Vector2f& GetPosition() const { return m_position; }
    const sf::Vector2f& GetSize() const { return m_size; }
    const std::string& GetName() const { return m_name; }
    const std::string& GetType() const { return m_type; }
    float GetRotation() const { return m_rotation; }
    bool IsVisible() const { return m_visible; }
	// properties
	const std::vector<MapObjectProperty>& GetProperties() const { return m_properties; }
	//get a property by name
	const MapObjectProperty* GetPropertyByName(const std::string& name) const;
    
    float GetX() const { return m_position.x; }
    float GetY() const { return m_position.y; }
    float GetWidth() const { return m_size.x; }
    float GetHeight() const { return m_size.y; }

    // Setters
    void SetGid(int gid) { m_gid = gid; }
    void SetPosition(const sf::Vector2f& position) { m_position = position; }
    void SetPosition(float x, float y) { m_position = sf::Vector2f(x, y); }
    void SetSize(const sf::Vector2f& size) { m_size = size; }
    void SetSize(float width, float height) { m_size = sf::Vector2f(width, height); }
    void SetName(const std::string& name) { m_name = name; }
    void SetType(const std::string& type) { m_type = type; }
    void SetRotation(float rotation) { m_rotation = rotation; }
    void SetVisible(bool visible) { m_visible = visible; }

	void AddProperty(const MapObjectProperty& property) { m_properties.push_back(property); }

    // Méthodes utilitaires
    sf::FloatRect GetBounds() const;
    bool Contains(const sf::Vector2f& point) const;
};


#endif // MAPOBJECT_H