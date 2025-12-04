#include "Tileset.h"

// ===== Tileset =====

Tileset::Tileset()
    : m_name("")
    , m_firstGid(0)
    , m_tileCount(0)
    , m_tileWidth(0)
    , m_tileHeight(0)
    , m_columns(0)
    , m_margin(0)
    , m_spacing(0)
{
}

Tileset::Tileset(const std::string& name, int firstGid)
    : m_name(name)
    , m_firstGid(firstGid)
    , m_tileCount(0)
    , m_tileWidth(0)
    , m_tileHeight(0)
    , m_columns(0)
    , m_margin(0)
    , m_spacing(0)
{
}

void Tileset::AddTile(int id, const std::string& imagePath, int imageWidth, int imageHeight)
{
    TileInfo tile;
    tile.id = id;
    tile.imagePath = imagePath;
    tile.imageSize = sf::Vector2i(imageWidth, imageHeight);
    tile.texture = nullptr;
    
    m_tiles[id] = tile;
}

const TileInfo* Tileset::GetTileInfo(int localId) const
{
    auto it = m_tiles.find(localId);
    if (it != m_tiles.end())
        return &it->second;
    return nullptr;
}

bool Tileset::HasTile(int localId) const
{
    return m_tiles.find(localId) != m_tiles.end();
}

bool Tileset::LoadTextures(const std::string& baseFolder)
{
    bool allLoaded = true;
    
    for (auto& pair : m_tiles)
    {
        int id = pair.first;
        TileInfo& tile = pair.second;
        
        std::string fullPath = baseFolder + tile.imagePath;
        
        if (m_textures.find(id) == m_textures.end())
        {
            sf::Texture texture;
            if (texture.loadFromFile(fullPath))
            {
                m_textures[id] = texture;
                tile.texture = &m_textures[id];
                //std::cout << "Texture chargée: " << fullPath << std::endl;
            }
            else
            {
                std::cerr << "Erreur de chargement de texture: " << fullPath << std::endl;
                allLoaded = false;
            }
        }
    }
    
    return allLoaded;
}

sf::Texture* Tileset::GetTexture(int localId)
{
    auto it = m_textures.find(localId);
    if (it != m_textures.end())
        return &it->second;
    return nullptr;
}

const sf::Texture* Tileset::GetTexture(int localId) const
{
    auto it = m_textures.find(localId);
    if (it != m_textures.end())
        return &it->second;
    return nullptr;
}

int Tileset::GidToLocalId(int gid) const
{
    return gid - m_firstGid;
}

int Tileset::LocalIdToGid(int localId) const
{
    return m_firstGid + localId;
}

bool Tileset::ContainsGid(int gid) const
{
    int localId = GidToLocalId(gid);
    return localId >= 0 && localId < m_tileCount;
}

void Tileset::Print() const
{
  /*  std::cout << "Tileset: " << m_name << std::endl;
    std::cout << "  First GID: " << m_firstGid << std::endl;
    std::cout << "  Tile Count: " << m_tileCount << std::endl;
    std::cout << "  Tile Size: " << m_tileWidth << "x" << m_tileHeight << std::endl;
    std::cout << "  Tiles: " << m_tiles.size() << std::endl;*/
    
    for (const auto& pair : m_tiles)
    {
        const TileInfo& tile = pair.second;
       /* std::cout << "    Tile " << pair.first << ": " << tile.imagePath 
                  << " (" << tile.imageSize.x << "x" << tile.imageSize.y << ")" << std::endl;*/
    }
}

// ===== TilesetManager =====

TilesetManager::TilesetManager()
{
}

void TilesetManager::AddTileset(const Tileset& tileset)
{
    m_tilesets.push_back(tileset);
    RebuildGidMap();
}

void TilesetManager::RebuildGidMap()
{
    m_gidToTileset.clear();
    
    for (auto& tileset : m_tilesets)
    {
        int firstGid = tileset.GetFirstGid();
        int lastGid = firstGid + tileset.GetTileCount();
        
        for (int gid = firstGid; gid < lastGid; ++gid)
        {
            m_gidToTileset[gid] = &tileset;
        }
    }
}

Tileset* TilesetManager::GetTilesetByGid(int gid)
{
    auto it = m_gidToTileset.find(gid);
    if (it != m_gidToTileset.end())
        return it->second;
    return nullptr;
}

const Tileset* TilesetManager::GetTilesetByGid(int gid) const
{
    auto it = m_gidToTileset.find(gid);
    if (it != m_gidToTileset.end())
        return it->second;
    return nullptr;
}

Tileset* TilesetManager::GetTilesetByName(const std::string& name)
{
    for (auto& tileset : m_tilesets)
    {
        if (tileset.GetName() == name)
            return &tileset;
    }
    return nullptr;
}

const Tileset* TilesetManager::GetTilesetByName(const std::string& name) const
{
    for (const auto& tileset : m_tilesets)
    {
        if (tileset.GetName() == name)
            return &tileset;
    }
    return nullptr;
}

bool TilesetManager::LoadAllTextures(const std::string& baseFolder)
{
    bool allLoaded = true;
    for (auto& tileset : m_tilesets)
    {
        if (!tileset.LoadTextures(baseFolder))
            allLoaded = false;
    }
    return allLoaded;
}

sf::Texture* TilesetManager::GetTextureByGid(int gid)
{
    Tileset* tileset = GetTilesetByGid(gid);
    if (tileset)
    {
        int localId = tileset->GidToLocalId(gid);
        return tileset->GetTexture(localId);
    }
    return nullptr;
}

const sf::Texture* TilesetManager::GetTextureByGid(int gid) const
{
    const Tileset* tileset = GetTilesetByGid(gid);
    if (tileset)
    {
        int localId = tileset->GidToLocalId(gid);
        return tileset->GetTexture(localId);
    }
    return nullptr;
}

void TilesetManager::Clear()
{
    m_tilesets.clear();
    m_gidToTileset.clear();
}

void TilesetManager::Print() const
{
   /* std::cout << "=== TILESET MANAGER ===" << std::endl;
    std::cout << "Nombre de tilesets: " << m_tilesets.size() << std::endl;
    std::cout << "Nombre de mappings GID: " << m_gidToTileset.size() << std::endl;*/
    std::cout << std::endl;
    
    for (const auto& tileset : m_tilesets)
    {
        tileset.Print();
        std::cout << std::endl;
    }
}