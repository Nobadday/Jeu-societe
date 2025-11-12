#include "TextureAtlas.hpp"


#pragma region TextureAtlas
TextureAtlas::TextureAtlas(void) : sf::Texture(), AtlasContainer()
{

}


bool TextureAtlas::Create(unsigned _width, unsigned _height)
{
	if (this->sf::Texture::create(_width, _height))
	{
		this->ClearAtlasFrames();
		return true;
	}
	return false;
}

bool TextureAtlas::LoadFromFilePicture(const sf::String& _filePath, bool _resetAtlas, const sf::IntRect& _area)
{
	if (this->sf::Texture::loadFromFile(_filePath, _area))
	{
		if (_resetAtlas)
		{
			this->ClearAtlasFrames();
		}
		return true;
	}
	return false;
}
bool TextureAtlas::LoadFromFilePicture(const sf::Image& _image, bool _resetAtlas, const sf::IntRect& _area)
{
	if (this->sf::Texture::loadFromImage(_image, _area))
	{
		if (_resetAtlas)
		{
			this->ClearAtlasFrames();
		}
		return true;
	}
	return false;
}

bool TextureAtlas::LoadFromFile(const std::string& _fileName, TextureAtlas::ParseType _parseType)
{
	std::fstream file(_fileName);
	std::string dirPath = filetools::GetDirName(_fileName);
	bool success = this->LoadFromFile(file, dirPath, _parseType);
	file.close();
	return success;
}
bool TextureAtlas::LoadFromFile(std::fstream& _file, const std::string& _dirPath, TextureAtlas::ParseType _parseType)
{
	if (_file.fail())
	{
		return false;
	}

	this->ClearAtlasFrames();
	std::string textureName = "";

	// Parse the type
	switch (_parseType)
	{
		case PARSE_XML_SPARROW_V2:
			{
			rapidxml::file<> xmlFile(_file);
			rapidxml::xml_document<> doc;
			doc.parse<0>(xmlFile.data());

			rapidxml::xml_node<>* baseNode = doc.first_node("TextureAtlas");

			textureName = baseNode->first_attribute("imagePath")->value();

			this->m_frames.resize(baseNode->value_size());
			int i = 0;
			for (rapidxml::xml_node<>* child = baseNode->first_node(); child; child = child->next_sibling())
			{
				AtlasFrame& frame = this->m_frames[i];

				frame.m_name = child->first_attribute("name")->value();
				frame.m_textureRect.left = std::stoi(child->first_attribute("x")->value());
				frame.m_textureRect.top = std::stoi(child->first_attribute("y")->value());
				frame.m_textureRect.width = std::stoi(child->first_attribute("width")->value());
				frame.m_textureRect.height = std::stoi(child->first_attribute("height")->value());

				rapidxml::xml_attribute<>* attr;
				attr = child->first_attribute("frameX");
				if (attr != NULL)
				{
					frame.m_frameOffset.left = std::stoi(attr->value());
					frame.m_frameOffset.top = std::stoi(child->first_attribute("frameY")->value());
				}
				attr = child->first_attribute("frameWidth");
				if (attr != NULL)
				{
					frame.m_frameOffset.width = std::stoi(attr->value());
					frame.m_frameOffset.height = std::stoi(child->first_attribute("frameHeight")->value());
				}
				attr = child->first_attribute("rotated");
				if (attr != NULL)
				{
					frame.m_rotated = strcmp(attr->value(), "true") == 0;
				}

				i++;
			}

			}
			break;

		case PARSE_JSON_AUTO:
			{
			nlohmann::json jsonBase = nlohmann::json::parse(_file);
			
			if (jsonBase["frames"].type() == nlohmann::json::value_t::object)
			{
				return this->LoadFromFile(_file, _dirPath, PARSE_JSON_HASH);
			}
			return this->LoadFromFile(_file, _dirPath, PARSE_JSON_ARRAY);
			
			}
			break;

		case PARSE_JSON_HASH:
			{
			nlohmann::json jsonBase = nlohmann::json::parse(_file);
			textureName = (std::string)jsonBase["meta"]["image"];


			nlohmann::json& objects = jsonBase["frames"];

			int i = 0;
			this->m_frames.resize(objects.size());
			for (auto it = objects.begin(); it != objects.end(); ++it)
			{
				AtlasFrame& frame = this->m_frames[i];
				 
				nlohmann::json& frameData = it.value();

				frame.m_name = it.key();
				nlohmann::json& frameSize = frameData["frame"];
				frame.SetTextureRect(frameSize["x"], frameSize["y"], frameSize["w"], frameSize["h"]);
				frame.m_rotated = frameData.value("rotated", false);
				
				i++;
			}
			}
			break;

		case PARSE_JSON_ARRAY:
			{
			nlohmann::json jsonBase = nlohmann::json::parse(_file);

			textureName = (std::string)jsonBase["meta"]["image"];

			nlohmann::json& array = jsonBase["frames"];
			size_t arraySize = array.size();

			this->m_frames.resize(arraySize);
			for (int i = 0; i < arraySize; i++)
			{
				nlohmann::json& frameData = array[i];

				AtlasFrame& frame = this->m_frames[i];

				frame.m_name = frameData["filename"];
				nlohmann::json& frameSize = frameData["frame"];
				frame.SetTextureRect(frameSize["x"], frameSize["y"], frameSize["w"], frameSize["h"]);
				frame.m_rotated = frameData.value("rotated", false);
			}
			}
			break;

		default:
			printf("[WARNING] TextureAtlas : Failed to load \"%s\" : Unknown Parse Type (%d)\n", _dirPath.c_str(), _parseType);
			return false;
			break;
	}


	// Get the image path
	textureName = _dirPath + textureName;
	
	// Load the image
	if (!this->loadFromFile(textureName))
	{
		printf("[WARNING] TextureAtlas : Couldn't load the atlas's picture at \"%s\". Newly made atlas Data remains loaded.\n", textureName.c_str());
		// You can load the picture with "LoadFromFilePicture()"
	}
	return true;
}


int TextureAtlas::FindFrameGroupIndex(const std::string& _name)
{
	for (int i = 0; i < this->m_frames.size(); i++)
	{
		std::string& cmpStr = this->GetAtlasFrame(i).m_name;
		if (cmpStr.compare(0, _name.size(), _name) == 0)
		{
			return i;
		}
	}
	return -1;
}
int TextureAtlas::FindFrameGroupLenght(const std::string& _name, int _startIndex)
{
	int lenght = 0;
	bool startCount = false;
	for (int i = _startIndex; i < this->m_frames.size(); i++)
	{
		std::string& cmpStr = this->GetAtlasFrame(i).m_name;
		if (startCount)
		{
			if (cmpStr.compare(0, _name.size(), _name) != 0)
			{
				return lenght;
			}
			else
			{
				lenght++;
			}
		}
		else if (cmpStr.compare(0, _name.size(), _name) == 0)
		{
			startCount = true;
			lenght++;
		}

	}
	return lenght;
}
int TextureAtlas::FindFrameGroupIndexLast(const std::string& _name)
{
	int groupIndex = this->FindFrameGroupIndex(_name);
	return groupIndex + this->FindFrameGroupLenght(_name, groupIndex);
}
AtlasFrame& TextureAtlas::GetFrameByGroup(const std::string& _name, int _index)
{
	return this->GetAtlasFrame(this->FindFrameGroupIndex(_name) + _index);
}


const sf::Texture* TextureAtlas::GetTexture(void) const
{
	return this;
}
const sf::Texture& TextureAtlas::GetTextureRef(void) const
{
	return *this;
}
TextureAtlas::operator const sf::Texture* () const
{
	return this->GetTexture();
}
TextureAtlas::operator const sf::Texture& () const
{
	return this->GetTextureRef();
}


#pragma endregion


// Texture Atlases for C++ & SFML v1.1.2