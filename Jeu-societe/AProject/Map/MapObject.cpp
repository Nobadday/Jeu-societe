#include "MapObject.h"

MapObject::MapObject()
    : m_gid(0)
    , m_position(0.0f, 0.0f)
    , m_size(0.0f, 0.0f)
    , m_name("")
    , m_type("")
    , m_rotation(0.0f)
    , m_visible(true)
	, m_properties()
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
    , m_properties()
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
    , m_properties()
{
}

const MapObjectProperty* MapObject::GetPropertyByName(const std::string& name) const
{
    for (const auto& property : m_properties)
    {
        if (property.GetName() == name)
        {
            return &property;
        }
    }
	return nullptr;
}

MapObjectProperty::MapObjectProperty()
    : m_name("")
    , m_type("")
    , m_stringValue("")
    , m_value(-1)
{
}

sf::FloatRect MapObject::GetBounds() const
{
    return sf::FloatRect(m_position.x, m_position.y, m_size.x, m_size.y);
}

bool MapObject::Contains(const sf::Vector2f& point) const
{
    return GetBounds().contains(point);
}