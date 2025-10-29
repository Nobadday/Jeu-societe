#include "AssetManager.hpp"

#ifdef ASSET_MANAGER_DEBUG
#define DEBUG_PRINT printf
#else
#define DEBUG_PRINT //
#endif

#define DEFAULT_CONTAINER ""

#pragma region Global
const char* AssetPlaceholders[AssetManager::AssetType::ASSET_TYPES] =
{
	"PLACEHOLDER:Image",
	"PLACEHOLDER:Texture",
	"PLACEHOLDER:Font",
	"PLACEHOLDER:Sound Buffer",
	"PLACEHOLDER:Music",
	"PLACEHOLDER:Texture Atlas",
	"PLACEHOLDER:Texture Animated"
};
const char* AssetTypeName[AssetManager::AssetType::ASSET_TYPES] =
{
	"Image",
	"Texture",
	"Font",
	"Sound Buffer",
	"Music",
	"Texture Atlas",
	"Texture Animated"
};
bool AssetManager::AssetTypeIsValid(AssetType _type)
{
	return (_type >= 0) && (_type < AssetManager::AssetType::ASSET_TYPES);
}
const char* AssetManager::GetAssetTypeName(AssetType _type)
{
	if (AssetManager::AssetTypeIsValid(_type))
	{
		return AssetTypeName[_type];
	}
	return "Unknown";
}
const char* AssetManager::GetAssetTypePlaceholder(AssetType _type)
{
	return AssetPlaceholders[_type];
}
AssetManager::AssetType AssetManager::GetAssetTypeFromName(const std::string& _name)
{
	for (int i = 0; i < AssetType::ASSET_TYPES; i++)
	{
		if (_name == AssetTypeName[i])
		{
			return (AssetType)i;
		}
	}
	return AssetType::UNKNOWN;
}
#pragma endregion

#pragma region Container
AssetManager::Container::Container(void) :
m_name ("")
{
}
AssetManager::Container::~Container(void)
{
	this->m_assets.clear();
	this->m_assets.shrink_to_fit();
}

int AssetManager::Container::FindAssetIndex(const std::string& _name, AssetType _type)
{
	for (int i = 0; i < this->m_assets.size(); i++)
	{
		Asset& asset = this->m_assets[i];
		if ((asset.name == _name) && ((asset.type == _type) || (_type == AssetType::UNKNOWN)))
		{
			return i;
		}
	}
	return -1;
}
bool AssetManager::Container::AssetExists(const std::string& _name, AssetType _type)
{
	return this->FindAssetIndex(_name, _type) != -1;
}

AssetManager::Asset& AssetManager::Container::GetAssetWrap(int _index)
{
	return this->m_assets[_index];
}

//void*& AssetManager::Container::GetAsset(int _index)
//{
//	return this->GetAssetWrap(_index).object.get();
//}
//void*& AssetManager::Container::GetAsset(const std::string& _name, AssetType _type)
//{
//	return this->GetAsset(this->FindAssetIndex(_name, _type));
//}

void AssetManager::Container::DeleteAsset(int _index)
{
	this->m_assets.erase(this->m_assets.begin() + _index);
}

void AssetManager::Container::Clear()
{
	this->m_assets.clear();
	this->m_assets.shrink_to_fit();
}
#pragma endregion




#pragma region Manager

AssetManager::AssetManager(void)
{
	this->CreateContainer(DEFAULT_CONTAINER);
}

AssetManager::~AssetManager(void)
{
	this->Clear(false);
}

void AssetManager::Clear(void)
{
	this->Clear(true);
}


int AssetManager::CreateContainer(const std::string& _name)
{
	int index = this->FindContainerIndex(_name);
	if (index == -1)
	{
		index = (int)this->m_containers.size();
		this->m_containers.resize(index + 1);
		this->m_containers[index].m_name = _name;
	}
	return index;
}

void AssetManager::Clear(bool _secured)
{
	for (int i = ((int)this->m_containers.size()) - 1; i >= 0; i--)
	{
		this->DeleteContainer(i, _secured);
	}
}
void AssetManager::DeleteContainer(int _index, bool _secured)
{
	Container& container = this->m_containers[_index];
	container.Clear();

	if (!_secured && (_index != 0))
	{
		this->m_containers.erase(this->m_containers.begin()+_index);
	}
}
void AssetManager::DeleteContainer(int _index)
{
	this->DeleteContainer(_index, true);
}
void AssetManager::DeleteContainer(const std::string& _name)
{
	this->DeleteContainer(this->FindContainerIndex(_name));
}

void AssetManager::DeleteLastContainer(void)
{
	size_t size = this->m_containers.size();
	if (size >= 1)
	{
		this->DeleteContainer(((int)(size)) - 1);
	}
}

int AssetManager::FindContainerIndex(const std::string& _name)
{
	for (int i = 0; i < this->m_containers.size(); i++)
	{
		if (this->m_containers[i].m_name == _name)
		{
			return i;
		}
	}
	return -1;
}
bool AssetManager::ContainerExists(const std::string& _name)
{
	return FindContainerIndex(_name) != -1;
}
bool AssetManager::ContainerExists(int _index)
{
	return (_index >= 0) && (_index < this->m_containers.size());
}

bool AssetManager::LoadManifest(const std::string& _filePath, const std::string& _containerName)
{
	std::fstream file(_filePath);
	if (file.fail())
	{
		printf("[WARNING] AssetManager : Failed to load file\n");
		return false;
	}

	int containerIndex = this->CreateContainer(_containerName);
	nlohmann::json jsonBase = nlohmann::json::parse(file);

	nlohmann::json& array = jsonBase["assets"];
	size_t arraySize = array.size();
	for (int i = 0; i < arraySize; i++)
	{
		nlohmann::json& object = array[i];

		const std::string& assetName = object["name"];
		const std::string& assetPath = object["path"];
		AssetType assetType = (AssetType)object.value("type", (int)AssetType::UNKNOWN);


		void* clsObj = NULL;
		switch (assetType)
		{
			case AssetType::IMAGE:
				clsObj = new sf::Image();
				((sf::Image*)clsObj)->loadFromFile(assetPath);
				break;
			case AssetType::TEXTURE:
				clsObj = new sf::Texture();
				((sf::Texture*)clsObj)->loadFromFile(assetPath);
				break;
			case AssetType::FONT:
				clsObj = new sf::Font();
				((sf::Font*)clsObj)->loadFromFile(assetPath);
				break;
			case AssetType::SOUND_BUFFER:
				clsObj = new sf::SoundBuffer();
				((sf::SoundBuffer*)clsObj)->loadFromFile(assetPath);
				break;
			case AssetType::MUSIC:
				clsObj = new sf::Music();
				((sf::Music*)clsObj)->openFromFile(assetPath);
				break;

			case AssetType::TEXTURE_ATLAS:
				clsObj = new TextureAtlas();
				((TextureAtlas*)clsObj)->LoadFromFile(assetPath, (TextureAtlas::ParseType)object["subType"]);
				break;
			case AssetType::TEXTURE_ANIMATED:
				clsObj = new TextureAnimated();
				((TextureAnimated*)clsObj)->LoadFromFile(assetPath, (TextureAnimated::AnimationType)object["subType"]);
				break;

			default:
			case AssetType::UNKNOWN:
				printf("[WARNING] AssetManager : Impossible to create an object because an unknown asset type was given\n");
				break;
		}

	}
	return true;
}


AssetManager::Container& AssetManager::GetContainer(int _index)
{
	return this->m_containers[_index];
}
AssetManager::Container& AssetManager::GetContainer(const std::string& _name)
{
	return this->GetContainer(this->FindContainerIndex(_name));
}

void* AssetManager::GetAsset(const std::string& _name, AssetType _type)
{
	for (int i = ((int)this->m_containers.size())-1; i >= 0; i--)
	{
		Container& container = this->m_containers[i];
		int id = container.FindAssetIndex(_name, _type);
		if (id != -1)
		{
			return (void*)container.m_assets[id].object.get();
		}
	}
	return NULL;
}
void* AssetManager::GetAsset(const std::string& _name, AssetType _type, const std::string& _placeholder)
{
	void* content = this->GetAsset(_name, _type);
	if (content == NULL)
	{
		return this->GetAsset(_placeholder, _type);
	}
	return NULL;
}

void* AssetManager::GetAssetOrPlaceholder(const std::string& _name, AssetType _type)
{
	void* content = this->GetAsset(_name, _type);
	if ((content == NULL) && AssetTypeIsValid(_type))
	{
		return this->GetAsset(GetAssetTypePlaceholder(_type), _type);
	}
	return NULL;
}

#pragma endregion


// Asset Manager C++ v1.0