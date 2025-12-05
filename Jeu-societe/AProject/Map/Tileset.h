#pragma once

#include "../Common.hpp"

// Structure pour stocker les informations d'une tuile individuelle
struct TileInfo
{
    int id;                     // ID de la tuile dans le tileset
    std::string imagePath;      // Chemin de l'image
    sf::Vector2i imageSize;     // Taille de l'image (width, height)
    sf::Texture* texture;       // Pointeur vers la texture chargée

    TileInfo() 
        : id(0)
        , imageSize(0, 0)
        , texture(nullptr)
    {}
};

class Tileset
{
private:
    std::string m_name;                     // Nom du tileset
    int m_firstGid;                         // Premier GID du tileset
    int m_tileCount;                        // Nombre total de tuiles
    int m_tileWidth;                        // Largeur d'une tuile
    int m_tileHeight;                       // Hauteur d'une tuile
    int m_columns;                          // Nombre de colonnes
    int m_margin;                           // Marge autour du tileset
    int m_spacing;                          // Espacement entre les tuiles
    std::map<int, TileInfo> m_tiles;        // Map des tuiles (id -> TileInfo)
    std::map<int, sf::Texture> m_textures;  // Textures chargées

public:
    // Constructeur
    Tileset();
    Tileset(const std::string& _name, int _firstGid);

    // Getters
    const std::string& GetName() const { return m_name; }
    int GetFirstGid() const { return m_firstGid; }
    int GetTileCount() const { return m_tileCount; }
    int GetTileWidth() const { return m_tileWidth; }
    int GetTileHeight() const { return m_tileHeight; }
    int GetColumns() const { return m_columns; }
    int GetMargin() const { return m_margin; }
    int GetSpacing() const { return m_spacing; }
    
    // Setters
    void SetName(const std::string& _name) { m_name = _name; }
    void SetFirstGid(int _firstGid) { m_firstGid = _firstGid; }
    void SetTileCount(int _tileCount) { m_tileCount = _tileCount; }
    void SetTileWidth(int _width) { m_tileWidth = _width; }
    void SetTileHeight(int _height) { m_tileHeight = _height; }
    void SetColumns(int _columns) { m_columns = _columns; }
    void SetMargin(int _margin) { m_margin = _margin; }
    void SetSpacing(int _spacing) { m_spacing = _spacing; }

    // Gestion des tuiles
    void AddTile(int _id, const std::string& _imagePath, int _imageWidth, int _imageHeight);
    const TileInfo* GetTileInfo(int _localId) const;
    bool HasTile(int _localId) const;

    // Chargement des textures
    bool LoadTextures(const std::string& _baseFolder = "Assets/Map/");
    sf::Texture* GetTexture(int _localId);
    const sf::Texture* GetTexture(int _localId) const;

    // Conversion GID <-> ID local
    int GidToLocalId(int _gid) const;
    int LocalIdToGid(int _localId) const;
    bool ContainsGid(int _gid) const;

    // Debug
    void Print() const;
};

// Gestionnaire de tilesets avec mapping par GID
class TilesetManager
{
private:
    std::vector<Tileset> m_tilesets;        // Liste des tilesets
    std::map<int, Tileset*> m_gidToTileset; // Map GID -> Tileset correspondant

public:
    TilesetManager();

    // Ajouter un tileset
    void AddTileset(const Tileset& _tileset);
    
    // Récupérer un tileset par GID
    Tileset* GetTilesetByGid(int _gid);
    const Tileset* GetTilesetByGid(int _gid) const;
    
    // Récupérer un tileset par nom
    Tileset* GetTilesetByName(const std::string& _name);
    const Tileset* GetTilesetByName(const std::string& _name) const;

    // Récupérer tous les tilesets
    const std::vector<Tileset>& GetTilesets() const { return m_tilesets; }
    std::vector<Tileset>& GetTilesets() { return m_tilesets; }

    // Charger toutes les textures
    bool LoadAllTextures(const std::string& _baseFolder = "Assets/Map/");

    // Obtenir la texture correspondant à un GID
    sf::Texture* GetTextureByGid(int _gid);
    const sf::Texture* GetTextureByGid(int _gid) const;

    // Utilitaires
    size_t GetTilesetCount() const { return m_tilesets.size(); }
    void Clear();
    void Print() const;

private:
    void RebuildGidMap();
};