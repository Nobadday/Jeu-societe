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
const char* AssetManager::GetAssetTypeName(AssetType _type)
{
	if (_type < 0 || _type >= AssetManager::AssetType::ASSET_TYPES)
	{
		return "Unknown";
	}
	return AssetTypeName[_type];
}
#pragma endregion

#pragma region Container
AssetManager::Container::Container(void)
{
	this->m_name = "";
}
AssetManager::Container::~Container(void)
{
	this->m_assets.clear();
	this->m_assets.shrink_to_fit();
}

void AssetManager::Container::AddAsset(const std::string& _name, void* _object, AssetType _type)
{
	if (!this->AssetExists(_name, _type))
	{
		size_t index = this->m_assets.size();
		this->m_assets.resize(index + 1);
		Asset& asset = this->m_assets[index];
		asset.name = _name;
		asset.object = _object;
		asset.type = _type;
	}
	else
	{
		DEBUG_PRINT("[WARNING] Asset Manager : Name conflict with \"%s\" of type \"%s\"\n", _name.c_str(), AssetManager::GetAssetTypeName(_type));
	}
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

void*& AssetManager::Container::GetAsset(int _index)
{
	return this->GetAssetWrap(_index).object;
}
void*& AssetManager::Container::GetAsset(const std::string& _name, AssetType _type)
{
	return this->GetAsset(this->FindAssetIndex(_name, _type));
}

void AssetManager::Container::Clear()
{
	
}
#pragma endregion


#pragma region Manager
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

void AssetManager::UnloadContainer(int _index, bool _secured)
{
	Container& container = this->m_containers[_index];
	container.Clear();

	if (!_secured && (_index != 0))
	{
		this->m_containers.erase(this->m_containers.begin()+_index);
	}
}
bool AssetManager::ContainerExists(const std::string& _name)
{
	return FindContainerIndex(_name) != -1;
}
bool AssetManager::ContainerExists(int _index)
{
	return (_index >= 0) && (_index < this->m_containers.size());
}



AssetManager::AssetManager(void)
{
	this->CreateContainer(DEFAULT_CONTAINER);
}

AssetManager::~AssetManager(void)
{
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

void*& AssetManager::GetAsset(const std::string& _name, AssetType _type, void* _placeholder)
{
	for (int i = ((int)this->m_containers.size())-1; i >= 0; i--)
	{
		Container& container = this->m_containers[i];
		int id = container.FindAssetIndex(_name, _type);
		if (id != -1)
		{
			return (void*&)container.m_assets[id];
		}
	}
	return _placeholder;
}

#pragma endregion


// Asset Manager C++ v1.0