#include "JsonReader.h"
#include <sstream>
#include <fstream> 

JsonReader::JsonReader() : m_loaded(false)
{
}

JsonReader::~JsonReader()
{
}

bool JsonReader::LoadFromFile(const std::string& _filename)
{
    m_filename = _filename;
    m_loaded = false;

    try
    {
        std::ifstream file(_filename);
        if (!file.is_open())
        {
            std::cerr << "Impossible d'ouvrir le fichier: " << _filename << std::endl;
            return false;
        }

        file >> m_data;
        file.close();
        m_loaded = true;

        std::cout << "Fichier JSON chargé avec succès: " << _filename << std::endl;
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Erreur lors du chargement du JSON: " << e.what() << std::endl;
        return false;
    }
}

bool JsonReader::HasKey(const std::string& _key) const
{
    if (!m_loaded)
        return false;

    return m_data.contains(_key);
}

void JsonReader::Print() const
{
    if (!m_loaded)
    {
        std::cout << "Aucun fichier JSON chargé." << std::endl;
        return;
    }

    std::cout << m_data.dump(4) << std::endl; // Indentation de 4 espaces
}