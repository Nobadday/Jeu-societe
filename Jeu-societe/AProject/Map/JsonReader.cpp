#include "JsonReader.h"
#include <sstream>
#include <fstream> 

JsonReader::JsonReader() : m_loaded(false)
{
}

JsonReader::~JsonReader()
{
}

bool JsonReader::loadFromFile(const std::string& filename)
{
    m_filename = filename;
    m_loaded = false;

    try
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "Impossible d'ouvrir le fichier: " << filename << std::endl;
            return false;
        }

        file >> m_data;
        file.close();
        m_loaded = true;

        std::cout << "Fichier JSON chargé avec succès: " << filename << std::endl;
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Erreur lors du chargement du JSON: " << e.what() << std::endl;
        return false;
    }
}

bool JsonReader::hasKey(const std::string& key) const
{
    if (!m_loaded)
        return false;

    return m_data.contains(key);
}

void JsonReader::print() const
{
    if (!m_loaded)
    {
        std::cout << "Aucun fichier JSON chargé." << std::endl;
        return;
    }

    std::cout << m_data.dump(4) << std::endl; // Indentation de 4 espaces
}