#ifndef JSONREADER_H
#define JSONREADER_H

#include "../Common.hpp"

#include "nlohmann/json.hpp" // Ajoutez json.hpp à votre projet

using json = nlohmann::json;

class JsonReader
{
private:
    json m_data;
    bool m_loaded;
    std::string m_filename;

public:
    JsonReader();
    ~JsonReader();

    // Charger un fichier JSON
    bool LoadFromFile(const std::string& _filename);

    // Vérifier si le fichier est chargé
    bool IsLoaded() const { return m_loaded; }

    // Obtenir une valeur par clé (retourne une valeur par défaut si la clé n'existe pas)
    template<typename T>
    T GetValue(const std::string& _key, const T& _defaultValue) const;

    // Obtenir une valeur imbriquée (ex: "player/position/x")
    template<typename T>
    T GetNestedValue(const std::string& _path, const T& _defaultValue) const;

    // Vérifier si une clé existe
    bool HasKey(const std::string& _key) const;

    // Obtenir le JSON brut pour manipulation avancée
    const json& GetData() const { return m_data; }

    // Afficher le contenu du JSON (debug)
    void Print() const;
};

template<typename T>
inline T JsonReader::GetValue(const std::string& key, const T& defaultValue) const
{
    if (!m_loaded)
        return defaultValue;

    try
    {
        if (m_data.contains(key))
            return m_data[key].get<T>();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Erreur lors de la lecture de la clé '" << key << "': " << e.what() << std::endl;
    }

    return defaultValue;
}

template<typename T>
inline T JsonReader::GetNestedValue(const std::string& path, const T& defaultValue) const
{
    if (!m_loaded)
        return defaultValue;

    try
    {
        json current = m_data;
        std::string key;
        std::istringstream stream(path);

        while (std::getline(stream, key, '/'))
        {
            if (!current.contains(key))
                return defaultValue;
            current = current[key];
        }

        return current.get<T>();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Erreur lors de la lecture du chemin '" << path << "': " << e.what() << std::endl;
    }

    return defaultValue;
}

#endif // JSONREADER_H