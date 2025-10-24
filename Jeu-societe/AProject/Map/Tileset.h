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
    Tileset(const std::string& name, int firstGid);

    // Getters
    const std::string& getName() const { return m_name; }
    int getFirstGid() const { return m_firstGid; }
    int getTileCount() const { return m_tileCount; }
    int getTileWidth() const { return m_tileWidth; }
    int getTileHeight() const { return m_tileHeight; }
    int getColumns() const { return m_columns; }
    int getMargin() const { return m_margin; }
    int getSpacing() const { return m_spacing; }
    
    // Setters
    void setName(const std::string& name) { m_name = name; }
    void setFirstGid(int firstGid) { m_firstGid = firstGid; }
    void setTileCount(int tileCount) { m_tileCount = tileCount; }
    void setTileWidth(int width) { m_tileWidth = width; }
    void setTileHeight(int height) { m_tileHeight = height; }
    void setColumns(int columns) { m_columns = columns; }
    void setMargin(int margin) { m_margin = margin; }
    void setSpacing(int spacing) { m_spacing = spacing; }

    // Gestion des tuiles
    void addTile(int id, const std::string& imagePath, int imageWidth, int imageHeight);
    const TileInfo* getTileInfo(int localId) const;
    bool hasTile(int localId) const;

    // Chargement des textures
    bool loadTextures(const std::string& baseFolder = "Assets/Map/");
    sf::Texture* getTexture(int localId);
    const sf::Texture* getTexture(int localId) const;

    // Conversion GID <-> ID local
    int gidToLocalId(int gid) const;
    int localIdToGid(int localId) const;
    bool containsGid(int gid) const;

    // Debug
    void print() const;
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
    void addTileset(const Tileset& tileset);
    
    // Récupérer un tileset par GID
    Tileset* getTilesetByGid(int gid);
    const Tileset* getTilesetByGid(int gid) const;
    
    // Récupérer un tileset par nom
    Tileset* getTilesetByName(const std::string& name);
    const Tileset* getTilesetByName(const std::string& name) const;

    // Récupérer tous les tilesets
    const std::vector<Tileset>& getTilesets() const { return m_tilesets; }
    std::vector<Tileset>& getTilesets() { return m_tilesets; }

    // Charger toutes les textures
    bool loadAllTextures(const std::string& baseFolder = "Assets/");

    // Obtenir la texture correspondant à un GID
    sf::Texture* getTextureByGid(int gid);
    const sf::Texture* getTextureByGid(int gid) const;

    // Utilitaires
    size_t getTilesetCount() const { return m_tilesets.size(); }
    void clear();
    void print() const;

private:
    void rebuildGidMap();
};