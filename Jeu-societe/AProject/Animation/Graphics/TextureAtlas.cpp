#include "TextureAtlas.hpp"


AtlasFrame::AtlasFrame(void)
{
}

void AtlasFrame::SetTextureRect(int _x, int _y, int _w, int _h)
{
	this->m_textureRect = { _x, _y, _w, _h };
}


TextureAtlas::TextureAtlas(void)
{
}

TextureAtlas::~TextureAtlas(void)
{
	this->ClearAll();
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
	this->ClearAll();

	std::string textureName = "";

	nlohmann::json jsonBase;

	switch (_parseType)
	{
		case PARSE_XML_SPARROW_V2:
			{
				rapidxml::file<> xmlFile(_file);
				rapidxml::xml_document<> doc;
				doc.parse<0>(xmlFile.data());
				
				char* test = doc.first_node()->name();
				rapidxml::xml_node<>* baseNode = doc.first_node("TextureAtlas");
				
				textureName = baseNode->first_attribute("imagePath")->value();

				rapidxml::xml_attribute<>* attr;
				for (rapidxml::xml_node<>* child = baseNode->first_node(); child; child = child->next_sibling())
				{
					AtlasFrame* frame = new AtlasFrame();
					frame->m_name = child->first_attribute("name")->value();
					frame->m_textureRect.left = std::stoi(child->first_attribute("x")->value());
					frame->m_textureRect.top = std::stoi(child->first_attribute("y")->value());
					frame->m_textureRect.width = std::stoi(child->first_attribute("width")->value());
					frame->m_textureRect.height = std::stoi(child->first_attribute("height")->value());

					attr = child->first_attribute("frameX");
					if (attr != NULL)
					{
						frame->m_frameOffset.left = std::stoi(attr->value());
						frame->m_frameOffset.top = std::stoi(child->first_attribute("frameY")->value());
						frame->m_frameOffset.width = std::stoi(child->first_attribute("frameWidth")->value());
						frame->m_frameOffset.height = std::stoi(child->first_attribute("frameHeight")->value());
					}
					attr = child->first_attribute("rotated");
					if (attr != NULL)
					{
						frame->m_rotated = strcmp(attr->value(), "true") == 0;
					}
					
					
					this->m_frameList.Append(frame);
				}

			}
			break;

		case PARSE_JSON_AUTO:
			{
				jsonBase = nlohmann::json::parse(_file);
				
				if (jsonBase["frames"].type_name() == "object")
				{
					return this->LoadFromFile(_file, _dirPath, PARSE_JSON_HASH);
				}
				else
				{
					return this->LoadFromFile(_file, _dirPath, PARSE_JSON_ARRAY);
				}
			}
			break;

		case PARSE_JSON_HASH:
			{
				jsonBase = nlohmann::json::parse(_file);
				textureName = (std::string)jsonBase["meta"]["image"];


				nlohmann::json& objects = jsonBase["frames"];

				int i = 0;
				this->m_frameList.Reallocate((int)objects.size());
				for (auto it = objects.begin(); it != objects.end(); ++it)
				{
					AtlasFrame* frame = new AtlasFrame();

					nlohmann::json& frameData = it.value();

					frame->m_name = it.key();
					nlohmann::json& frameSize = frameData["frame"];
					frame->SetTextureRect(frameSize["x"], frameSize["y"], frameSize["w"], frameSize["h"]);
					frame->m_rotated = frameData["rotated"];
					this->m_frameList.Replace(i, frame);
					i++;
				}
			}
			break;

		case PARSE_JSON_ARRAY:
			{
				jsonBase = nlohmann::json::parse(_file);
				textureName = (std::string)jsonBase["meta"]["image"];

				nlohmann::json& array = jsonBase["frames"];
				int arraySize = (int)array.size();

				this->m_frameList.Reallocate(arraySize);
				for (int i = 0; i < arraySize; i++)
				{
					nlohmann::json& frameData = array[i];

					AtlasFrame* frame = new AtlasFrame();

					frame->m_name = frameData["filename"];
					nlohmann::json& frameSize = frameData["frame"];
					frame->SetTextureRect(frameSize["x"], frameSize["y"], frameSize["w"], frameSize["h"]);
					frame->m_rotated = frameData["rotated"];
					this->m_frameList.Replace(i, frame);
				}
			}
			break;

		default:
			return false;
			break;
	}

	// Open the image

	m_texture.loadFromFile(_dirPath + textureName);

	return true;
}



AtlasFrame* TextureAtlas::GetFrame(int _index)
{
	return (AtlasFrame*)this->m_frameList.GetElement(_index);
}

AtlasFrame* TextureAtlas::GetFrame(const std::string& _name)
{
	for (int i = this->m_frameList.GetLen()-1; i >= 0; i--)
	{
		AtlasFrame* framePTR = this->GetFrame(i);
		if (framePTR->m_name == _name)
		{
			return framePTR;
		}
	}
	return NULL;
}

int TextureAtlas::GetFrameCount()
{
	return m_frameList.Len();
}

int TextureAtlas::FindFrameIndex(const std::string& _name)
{
	for (int i = this->m_frameList.GetLen() - 1; i >= 0; i--)
	{
		if (this->GetFrame(i)->m_name == _name)
		{
			return i;
		}
	}
	return -1;
}

bool TextureAtlas::FrameExists(const std::string& _name)
{
	return this->FindFrameIndex(_name) != -1;
}

const sf::Texture* TextureAtlas::GetTexture(void)
{
	return &this->m_texture;
}

const sf::Texture& TextureAtlas::GetTextureRef(void)
{
	return this->m_texture;
}

void TextureAtlas::ClearAll(void)
{
	for (int i = this->m_frameList.Len()-1; i >= 0; i--)
	{
		delete (AtlasFrame*)m_frameList.Pop(i);
	}
}

int TextureAtlas::FindFrameGroupIndex(const std::string& _name)
{
	for (int i = 0; i < this->m_frameList.GetLen(); i++)
	{
		std::string& cmpStr = this->GetFrame(i)->m_name;
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
	for (int i = _startIndex; i < this->m_frameList.GetLen(); i++)
	{
		std::string& cmpStr = this->GetFrame(i)->m_name;
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

AtlasFrame* TextureAtlas::GetFrameByGroup(const std::string& _name, int _index)
{
	return this->GetFrame(this->FindFrameGroupIndex(_name) + _index);
}

// Texture Atlases for C++ & SFML v1.0