#include "MapObject.h"

MapObject::MapObject()
    : m_gid(0)
    , m_position(0.0f, 0.0f)
    , m_size(0.0f, 0.0f)
    , m_name("")
    , m_type("")
    , m_rotation(0.0f)
    , m_visible(true)
{
}

MapObject::MapObject(int gid, const sf::Vector2f& position, const sf::Vector2f& size)
    : m_gid(gid)
    , m_position(position)
    , m_size(size)
    , m_name("")
    , m_type("")
    , m_rotation(0.0f)
    , m_visible(true)
{
}

MapObject::MapObject(int gid, float x, float y, float width, float height)
    : m_gid(gid)
    , m_position(x, y)
    , m_size(width, height)
    , m_name("")
    , m_type("")
    , m_rotation(0.0f)
    , m_visible(true)
{
}

sf::FloatRect MapObject::getBounds() const
{
    return sf::FloatRect(m_position.x, m_position.y, m_size.x, m_size.y);
}

bool MapObject::contains(const sf::Vector2f& point) const
{
    return getBounds().contains(point);
}