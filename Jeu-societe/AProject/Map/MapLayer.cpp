#include "MapLayer.h"
#include "Tileset.h"
//#include "Camera.h"

MapLayer::MapLayer()
    : m_id(0)
    , m_name("")
    , m_type("")
    , m_visible(true)
    , m_opacity(1.0f)
    , m_parallaxX(1.0f)
    , m_parallaxY(1.0f)
    , m_tintColor(sf::Color::White)
{
}

MapLayer::MapLayer(int _id, const std::string& _name, const std::string& _type)
    : m_id(_id)
    , m_name(_name)
    , m_type(_type)
    , m_visible(true)
    , m_opacity(1.0f)
    , m_parallaxX(1.0f)
    , m_parallaxY(1.0f)
    , m_tintColor(sf::Color::White)
{
}

void MapLayer::AddObject(const MapObject& _object)
{
    m_objects.push_back(_object);
}

MapObject& MapLayer::GetObject(size_t _index)
{
    return m_objects[_index];
}

MapObject& MapLayer::GetObject(std::string _name)
{
    for (int i = 0; i < m_objects.size(); i++)
    {
        if (_name == m_objects[i].GetName())
        {
            return m_objects[i];
        }
    }
    return m_objects[0];
}

const MapObject& MapLayer::GetObject(size_t _index) const
{
    return m_objects[_index];
}

void MapLayer::Draw(sf::RenderTarget& _target, TilesetManager& _tilesetManager, const sf::Vector2f& _camera) const
{
    if (!m_visible)
        return;

    sf::Vector2f parallax = sf::Vector2f(m_parallaxX - 1.f, m_parallaxY - 1.f );

    // Calculer l'offset de la caméra avec parallaxe
    float parallaxOffsetX = _camera.x * parallax.x;
    float parallaxOffsetY = _camera.y * parallax.y;

    for (const auto& object : m_objects)
    {
        if (!object.IsVisible())
            continue;

        int gid = object.GetGid();
        if (gid == 0)
            continue;

        // Obtenir la texture correspondante au GID
        sf::Texture* texture = _tilesetManager.GetTextureByGid(gid);
        if (!texture)
            continue;

        // Créer un sprite
        sf::Sprite sprite;
        sprite.setTexture(*texture);

        // Calculer la position avec parallaxe
        float worldX = object.GetX();
        float worldY = object.GetY();

        // Dans Tiled, l'origine Y est en bas de l'objet pour les images
        worldY -= object.GetHeight();

        // Appliquer la transformation de la caméra avec parallaxe
        float screenX = worldX - parallaxOffsetX;
        float screenY = worldY - parallaxOffsetY;

		// Correction : calculer le rectangle de la texture pour afficher que se qui est dans la caméra 
        
		/*sf::IntRect cameraBouns;

        sprite.setTextureRect(cameraBouns);*/

        sprite.setPosition(screenX, screenY);

        // Appliquer la taille
        float scaleX = object.GetWidth() / texture->getSize().x;
        float scaleY = object.GetHeight() / texture->getSize().y;
        sprite.setScale(scaleX, scaleY);

        // Appliquer la rotation
        if (object.GetRotation() != 0.0f)
        {
            sprite.setRotation(object.GetRotation());
        }

        // Appliquer l'opacité et la couleur de teinte
        sf::Color color = m_tintColor;
        color.a = static_cast<sf::Uint8>(m_opacity * 255);
        sprite.setColor(color);

        // Dessiner le sprite
        _target.draw(sprite);
    }
}