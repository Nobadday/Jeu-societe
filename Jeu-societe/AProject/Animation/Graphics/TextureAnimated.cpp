#include "TextureAnimated.hpp"

AnimationProperties::AnimationProperties(void) :
m_name (""),
m_atlasName(""),
m_atlasIndex (-1),
m_frameCount (0),
m_framerate (24.0f),
m_loop (false),
m_offset(0, 0)
{
}


TextureAnimated::TextureAnimated(void)
{
}

TextureAnimated::~TextureAnimated(void)
{
	this->ClearAll();
}

bool TextureAnimated::LoadFromFile(const std::string& _fileName, TextureAnimated::AnimationType _animationFormat)
{
	std::fstream file(_fileName);
	std::string dirPath = filetools::GetDirName(_fileName);
	bool success = this->LoadFromFile(file, dirPath, _animationFormat);
	file.close();
	return success;
}

bool TextureAnimated::LoadFromFile(std::fstream& _file, const std::string& _dirPath, TextureAnimated::AnimationType _animationFormat)
{
	if (_file.fail())
	{
		return false;
	}
	this->ClearAll();

	nlohmann::json jsonBase;
	

	switch (_animationFormat)
	{
		case ANIMATION_TEXANIM:
			{
				jsonBase = nlohmann::json::parse(_file);
				this->m_textureAtlas.LoadFromFile(_dirPath + (std::string)jsonBase["filePath"], TextureAtlas::PARSE_JSON_ARRAY);

				nlohmann::json& array = jsonBase["animations"];
				int arraySize = (int)array.size();
				this->m_animations.Reallocate(arraySize);
				for (int i = 0; i < arraySize; i++)
				{
					nlohmann::json& object = array[i];

					AnimationProperties* newAnim = new AnimationProperties();
					newAnim->m_name = object["animationName"];
					newAnim->m_atlasName = object.value("texName", newAnim->m_name);
					newAnim->m_loop = object.value("loop", false);
					newAnim->m_framerate = object.value("fps", 24.0f);

					newAnim->m_atlasIndex = this->m_textureAtlas.FindFrameGroupIndex(newAnim->m_atlasName);
					newAnim->m_frameCount = this->m_textureAtlas.FindFrameGroupLenght(newAnim->m_atlasName, newAnim->m_atlasIndex);
					this->m_animations.Replace(i, newAnim);
				}
			}
			break;

		case ANIMATION_ANIM:
			{
				jsonBase = nlohmann::json::parse(_file);
				this->m_textureAtlas.LoadFromFile(_dirPath + (std::string)jsonBase["assetPath"], TextureAtlas::PARSE_JSON_ARRAY);

				nlohmann::json& array = jsonBase["animations"];
				int arraySize = (int)array.size();
				this->m_animations.Reallocate(arraySize);
				for (int i = 0; i < arraySize; i++)
				{
					nlohmann::json& object = array[i];

					AnimationProperties* newAnim = new AnimationProperties();
					newAnim->m_name = object["name"];
					newAnim->m_atlasName = object.value("atlas", newAnim->m_name);
					newAnim->m_loop = object.value("loop", false);
					newAnim->m_framerate = object.value("fps", (float)jsonBase["framerate"]);
					

					newAnim->m_atlasIndex = this->m_textureAtlas.FindFrameGroupIndex(newAnim->m_atlasName);
					newAnim->m_frameCount = this->m_textureAtlas.FindFrameGroupLenght(newAnim->m_atlasName, newAnim->m_atlasIndex);
					this->m_animations.Replace(i, newAnim);
				}
			}
		break;

		case ANIMATION_ASEPRITE:
			{
				jsonBase = nlohmann::json::parse(_file);
				nlohmann::json& meta = jsonBase["meta"];
				this->m_textureAtlas.LoadFromFile(_file, _dirPath, TextureAtlas::PARSE_JSON_AUTO);
				nlohmann::json& otherArray = jsonBase["frames"];

				nlohmann::json& array = meta["frameTags"];
				int arraySize = (int)array.size();
				this->m_animations.Reallocate(arraySize);
				for (int i = 0; i < arraySize; i++)
				{
					nlohmann::json& object = array[i];

					AnimationProperties* newAnim = new AnimationProperties();
					newAnim->m_name = object["name"];
					newAnim->m_loop = object.value("loop", false);
					newAnim->m_atlasIndex = object["from"];
					newAnim->m_frameCount = (object["to"] - newAnim->m_atlasIndex) + 1;
					
					newAnim->m_framerate = anim::AniMath::MilisecondsToFPS(otherArray[newAnim->m_atlasIndex]["duration"]);

					
					this->m_animations.Replace(i, newAnim);
				}
			}
			break;

		case ANIMATION_FNF_LEGACY:
		{
			jsonBase = nlohmann::json::parse(_file);
			this->m_textureAtlas.LoadFromFile(_dirPath + (std::string)jsonBase["image"] + ".xml", TextureAtlas::PARSE_XML_SPARROW_V2);

			nlohmann::json& array = jsonBase["animations"];
			int arraySize = (int)array.size();
			this->m_animations.Reallocate(arraySize);
			for (int i = 0; i < arraySize; i++)
			{
				nlohmann::json& object = array[i];

				AnimationProperties* newAnim = new AnimationProperties();
				newAnim->m_name = object["anim"];
				newAnim->m_atlasName = object.value("name", newAnim->m_name);
				newAnim->m_loop = object.value("loop", false);
				newAnim->m_framerate = object.value("fps", 24.0f);

				if (object.contains("offsets"))
				{
					nlohmann::json& offsets = object["offsets"];
					newAnim->m_offset.x = offsets[0];
					newAnim->m_offset.y = offsets[1];
				}

				newAnim->m_atlasIndex = this->m_textureAtlas.FindFrameGroupIndex(newAnim->m_atlasName);
				newAnim->m_frameCount = this->m_textureAtlas.FindFrameGroupLenght(newAnim->m_atlasName, newAnim->m_atlasIndex);
				this->m_animations.Replace(i, newAnim);
			}
		}
		break;

		case ANIMATION_FNF:
		{
			jsonBase = nlohmann::json::parse(_file);
			this->m_textureAtlas.LoadFromFile(_dirPath + (std::string)jsonBase["assetPath"] + ".xml", TextureAtlas::PARSE_XML_SPARROW_V2);

			nlohmann::json& array = jsonBase["animations"];
			int arraySize = (int)array.size();
			this->m_animations.Reallocate(arraySize);
			for (int i = 0; i < arraySize; i++)
			{
				nlohmann::json& object = array[i];

				AnimationProperties* newAnim = new AnimationProperties();
				newAnim->m_name = object["name"];
				newAnim->m_atlasName = object.value("prefix", newAnim->m_name);
				newAnim->m_loop = object.value("loop", false);
				newAnim->m_framerate = object.value("fps", 24.0f);


				newAnim->m_atlasIndex = this->m_textureAtlas.FindFrameGroupIndex(newAnim->m_atlasName);
				newAnim->m_frameCount = this->m_textureAtlas.FindFrameGroupLenght(newAnim->m_atlasName, newAnim->m_atlasIndex);
				this->m_animations.Replace(i, newAnim);
			}
		}
		break;

		default:
			return false;
			break;

	}

	return true;
}


bool TextureAnimated::LoadFromFile(const std::string& _fileName, TextureAtlas::ParseType _atlasParseType)
{
	std::fstream file(_fileName);
	std::string dirPath = filetools::GetDirName(_fileName);
	bool success = this->LoadFromFile(file, dirPath, _atlasParseType);
	file.close();
	return success;
}

bool TextureAnimated::LoadFromFile(std::fstream& _file, const std::string& _dirPath, TextureAtlas::ParseType _atlasParseType)
{
	if (_file.fail())
	{
		return false;
	}
	this->ClearAll();


	this->m_textureAtlas.LoadFromFile(_file, _dirPath, _atlasParseType);
	
	AnimationProperties* newAnim = new AnimationProperties();
	newAnim->m_atlasName = "";
	newAnim->m_atlasIndex = 0;
	newAnim->m_frameCount = this->m_textureAtlas.GetFrameCount();
	newAnim->m_framerate = 24.0f;

	this->m_animations.Append(newAnim);
	return true;
}

void TextureAnimated::ClearAll(void)
{
	for (int i = this->m_animations.Len() - 1; i >= 0; i--)
	{
		delete (AtlasFrame*)m_animations.Pop(i);
	}
}

AnimationProperties* TextureAnimated::GetAnimation(int _index)
{
	return (AnimationProperties*)this->m_animations[_index];
}

AnimationProperties* TextureAnimated::GetAnimation(const std::string& _name)
{
	AnimationProperties* anim;
	for (int i = 0; i < this->m_animations.Len(); i++)
	{
		anim = (AnimationProperties*)this->m_animations[i];
		if (anim->m_name == _name)
		{
			return anim;
		}
	}
	return NULL;
}

int TextureAnimated::FindAnimationIndex(const std::string& _name)
{
	for (int i = 0; i < this->m_animations.Len(); i++)
	{
		if (((AnimationProperties*)this->m_animations[i])->m_name == _name)
		{
			return i;
		}
	}
	return -1;
}

bool TextureAnimated::AnimationExists(const std::string& _name)
{
	return this->FindAnimationIndex(_name) != -1;
}

bool TextureAnimated::AnimationExists(int _index)
{
	return this->m_animations.IndexExists(_index);
}

AtlasFrame* TextureAnimated::GetFrame(int _atlasIndex, int _frame)
{
	return this->m_textureAtlas.GetFrame(_atlasIndex + _frame);
}

const sf::Texture* TextureAnimated::GetTexture(void)
{
	return this->m_textureAtlas.GetTexture();
}
const sf::Texture& TextureAnimated::GetTextureRef(void)
{
	return this->m_textureAtlas.GetTextureRef();
}

int TextureAnimated::GetAnimationCount(void)
{
	return this->m_animations.Len();
}

// Texture Animated v1.0