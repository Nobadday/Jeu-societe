#include "TextureAnimated.hpp"



TextureAnimated::AnimationProperties::AnimationProperties(void) :
m_name		 (""),
m_atlasName	 (""),
m_atlasIndex (-1),
m_frameCount (0),
m_framerate  (ANIMATION_DEFAULT_FPS),
m_loop		 (ANIMATION_DEFAULT_LOOP),
m_offset	 (0, 0)
{

}


#pragma region Texture Animated
TextureAnimated::TextureAnimated(void) :
m_textureAtlas (),
m_animations ()
{
	
}

TextureAnimated::~TextureAnimated(void)
{
	this->ClearAnimations();
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
	this->ClearAnimations();
	

	switch (_animationFormat)
	{
		case ANIMATION_TEXANIM:
			{
			nlohmann::json jsonBase = nlohmann::json::parse(_file);
			this->m_textureAtlas.LoadFromFile(_dirPath + (std::string)jsonBase["filePath"], TextureAtlas::PARSE_JSON_ARRAY);

			nlohmann::json& array = jsonBase["animations"];
			size_t arraySize = array.size();
			this->m_animations.resize(arraySize);
			for (int i = 0; i < arraySize; i++)
			{
				nlohmann::json& object = array[i];

				AnimationProperties& newAnim = this->m_animations[i];
				newAnim.m_name = object["animationName"];
				newAnim.m_atlasName = object.value("texName", newAnim.m_name);
				newAnim.m_loop = object.value("loop", ANIMATION_DEFAULT_LOOP);
				newAnim.m_framerate = object.value("fps", ANIMATION_DEFAULT_FPS);

				newAnim.m_atlasIndex = this->m_textureAtlas.FindFrameGroupIndex(newAnim.m_atlasName);
				newAnim.m_frameCount = this->m_textureAtlas.FindFrameGroupLenght(newAnim.m_atlasName, newAnim.m_atlasIndex);
			}
			}
			break;

		case ANIMATION_ANIM:
			{
			nlohmann::json jsonBase = nlohmann::json::parse(_file);
			TextureAtlas::ParseType parseType = (TextureAtlas::ParseType)jsonBase.value("parseType", TextureAtlas::ParseType::PARSE_JSON_ARRAY);
			this->m_textureAtlas.LoadFromFile(_dirPath + (std::string)jsonBase["assetPath"], parseType);

			float baseFPS = (float)jsonBase.value("framerate", ANIMATION_DEFAULT_FPS);
			nlohmann::json& array = jsonBase["animations"];
			size_t arraySize = array.size();
			this->m_animations.resize(arraySize);
			for (int i = 0; i < arraySize; i++)
			{
				nlohmann::json& object = array[i];

				AnimationProperties& newAnim = this->m_animations[i];
				newAnim.m_name = object["name"];
				newAnim.m_atlasName = object.value("atlas", newAnim.m_name);
				newAnim.m_loop = object.value("loop", ANIMATION_DEFAULT_LOOP);
				newAnim.m_framerate = object.value("fps", baseFPS);
				

				newAnim.m_atlasIndex = this->m_textureAtlas.FindFrameGroupIndex(newAnim.m_atlasName);
				newAnim.m_frameCount = this->m_textureAtlas.FindFrameGroupLenght(newAnim.m_atlasName, newAnim.m_atlasIndex);
			}
			}
			break;

		case ANIMATION_ASEPRITE:
			{
			nlohmann::json jsonBase = nlohmann::json::parse(_file);

			nlohmann::json& meta = jsonBase["meta"];
			this->m_textureAtlas.LoadFromFile(_file, _dirPath, TextureAtlas::PARSE_JSON_AUTO);
			nlohmann::json& otherArray = jsonBase["frames"];

			nlohmann::json& array = meta["frameTags"];
			size_t arraySize = array.size();
			this->m_animations.resize(arraySize);
			for (int i = 0; i < arraySize; i++)
			{
				nlohmann::json& object = array[i];

				AnimationProperties& newAnim = this->m_animations[i];
				newAnim.m_name = object["name"];
				newAnim.m_loop = object.value("loop", ANIMATION_DEFAULT_LOOP);
				newAnim.m_atlasIndex = object["from"];
				newAnim.m_frameCount = (object["to"] - newAnim.m_atlasIndex) + 1;
				

				newAnim.m_framerate = anim::AniMath::MilisecondsToFPS(otherArray[newAnim.m_atlasIndex]["duration"]);
			}
			}
			break;

		case ANIMATION_FNF_LEGACY:
			{
			nlohmann::json jsonBase = nlohmann::json::parse(_file);
			this->m_textureAtlas.LoadFromFile(_dirPath + (std::string)jsonBase["image"] + ".xml", TextureAtlas::PARSE_XML_SPARROW_V2);

			nlohmann::json& array = jsonBase["animations"];
			size_t arraySize = array.size();
			this->m_animations.resize(arraySize);
			for (int i = 0; i < arraySize; i++)
			{
				nlohmann::json& object = array[i];

				AnimationProperties& newAnim = this->m_animations[i];

				newAnim.m_name = object["anim"];
				newAnim.m_atlasName = object.value("name", newAnim.m_name);
				newAnim.m_loop = object.value("loop", ANIMATION_DEFAULT_LOOP);
				newAnim.m_framerate = object.value("fps", ANIMATION_DEFAULT_FPS);

				if (object.contains("offsets"))
				{
					nlohmann::json& offsets = object["offsets"];
					newAnim.m_offset.x = offsets[0];
					newAnim.m_offset.y = offsets[1];
				}

				newAnim.m_atlasIndex = this->m_textureAtlas.FindFrameGroupIndex(newAnim.m_atlasName);
				newAnim.m_frameCount = this->m_textureAtlas.FindFrameGroupLenght(newAnim.m_atlasName, newAnim.m_atlasIndex);
			}
			}
			break;

		case ANIMATION_FNF:
			{
			nlohmann::json jsonBase = nlohmann::json::parse(_file);
			this->m_textureAtlas.LoadFromFile(_dirPath + (std::string)jsonBase["assetPath"] + ".xml", TextureAtlas::PARSE_XML_SPARROW_V2);

			nlohmann::json& array = jsonBase["animations"];
			size_t arraySize = array.size();
			this->m_animations.resize(arraySize);
			for (int i = 0; i < arraySize; i++)
			{
				nlohmann::json& object = array[i];

				AnimationProperties& newAnim = this->m_animations[i];
				newAnim.m_name = object["name"];
				newAnim.m_atlasName = object.value("prefix", newAnim.m_name);
				newAnim.m_loop = object.value("loop", ANIMATION_DEFAULT_LOOP);
				newAnim.m_framerate = object.value("fps", ANIMATION_DEFAULT_FPS);


				newAnim.m_atlasIndex = this->m_textureAtlas.FindFrameGroupIndex(newAnim.m_atlasName);
				newAnim.m_frameCount = this->m_textureAtlas.FindFrameGroupLenght(newAnim.m_atlasName, newAnim.m_atlasIndex);
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
	this->ClearAnimations();


	if (this->m_textureAtlas.LoadFromFile(_file, _dirPath, _atlasParseType))
	{
		size_t index = this->m_animations.size();
		this->m_animations.resize(index + 1);
		AnimationProperties& newAnim = this->m_animations[index];
		newAnim.m_atlasName = "";
		newAnim.m_atlasIndex = 0;
		newAnim.m_frameCount = (int)this->m_textureAtlas.GetAtlasFrameCount();
		newAnim.m_framerate = ANIMATION_DEFAULT_FPS;
		newAnim.m_loop = ANIMATION_DEFAULT_LOOP;
	}
	

	return true;
}


void TextureAnimated::ClearAnimations(void)
{
	this->m_animations.clear();
	this->m_animations.shrink_to_fit();
}

TextureAnimated::AnimationProperties& TextureAnimated::GetAnimation(int _index)
{
	return this->m_animations[_index];
}

TextureAnimated::AnimationProperties& TextureAnimated::GetAnimation(const std::string& _name)
{
	return this->m_animations[this->FindAnimationIndex(_name)];
}

int TextureAnimated::FindAnimationIndex(const std::string& _name)
{
	for (int i = 0; i < this->m_animations.size(); i++)
	{
		if (this->m_animations[i].m_name == _name)
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
	return (_index >= 0) && (_index < this->m_animations.size());
}


AtlasFrame& TextureAnimated::GetFrame(int _atlasIndex, int _frame)
{
	return this->m_textureAtlas.GetAtlasFrame(_atlasIndex + _frame);
}


const TextureAtlas& TextureAnimated::GetTextureAtlas(void)
{
	return this->m_textureAtlas;
}
const sf::Texture& TextureAnimated::GetTexture(void)
{
	return this->m_textureAtlas.GetTextureRef();
}

size_t TextureAnimated::GetAnimationCount(void)
{
	return this->m_animations.size();
}

#pragma endregion


// Texture Animated v1.2.3