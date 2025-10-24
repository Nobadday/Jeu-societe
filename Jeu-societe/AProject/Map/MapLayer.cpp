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

MapLayer::MapLayer(int id, const std::string& name, const std::string& type)
    : m_id(id)
    , m_name(name)
    , m_type(type)
    , m_visible(true)
    , m_opacity(1.0f)
    , m_parallaxX(1.0f)
    , m_parallaxY(1.0f)
    , m_tintColor(sf::Color::White)
{
}

void MapLayer::AddObject(const MapObject& object)
{
    m_objects.push_back(object);
}

MapObject& MapLayer::GetObject(size_t index)
{
    return m_objects[index];
}

const MapObject& MapLayer::GetObject(size_t index) const
{
    return m_objects[index];
}

void MapLayer::Draw(sf::RenderTarget& target, TilesetManager& tilesetManager, const sf::Vector2f& camera) const
{
    if (!m_visible)
        return;

    // Calculer l'offset de la caméra avec parallaxe
    float parallaxOffsetX = camera.x * m_parallaxX;
    float parallaxOffsetY = camera.y * m_parallaxY;

    for (const auto& object : m_objects)
    {
        if (!object.isVisible())
            continue;

        int gid = object.getGid();
        if (gid == 0)
            continue;

        // Obtenir la texture correspondante au GID
        sf::Texture* texture = tilesetManager.getTextureByGid(gid);
        if (!texture)
            continue;

        // Créer un sprite
        sf::Sprite sprite;
        sprite.setTexture(*texture);

        // Calculer la position avec parallaxe
        float worldX = object.getX();
        float worldY = object.getY();

        // Dans Tiled, l'origine Y est en bas de l'objet pour les images
        worldY -= object.getHeight();

        // Appliquer la transformation de la caméra avec parallaxe
        float screenX = worldX - parallaxOffsetX;
        float screenY = worldY - parallaxOffsetY;

        sprite.setPosition(screenX, screenY);

        // Appliquer la taille
        float scaleX = object.getWidth() / texture->getSize().x;
        float scaleY = object.getHeight() / texture->getSize().y;
        sprite.setScale(scaleX, scaleY);

        // Appliquer la rotation
        if (object.getRotation() != 0.0f)
        {
            sprite.setRotation(object.getRotation());
        }

        // Appliquer l'opacité et la couleur de teinte
        sf::Color color = m_tintColor;
        color.a = static_cast<sf::Uint8>(m_opacity * 255);
        sprite.setColor(color);

        // Dessiner le sprite
        target.draw(sprite);
    }
}