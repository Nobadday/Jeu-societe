#include "AssetManager.hpp"

#ifdef ASSET_MANAGER_DEBUG
#define DEBUG_PRINT printf
#else
#define DEBUG_PRINT //
#endif

#define DEFAULT_MANIFEST ""

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
	return AssetTypeName[_type];
}



int AssetManager::FindManifestIndex(const std::string& _name)
{
	for (int i = 0; i < this->m_manifests.size(); i++)
	{
		if (this->m_manifests[i].m_name == _name)
		{
			return i;
		}
	}
	return -1;
}

bool AssetManager::UnloadManifest(int _index, bool _secured)
{
	if (_secured && (_index == 0))
	{
		return false;
	}

	Containers& manif = this->m_manifests[_index];
	
	
	return true;
}
bool AssetManager::ContainerExists(const std::string& _name)
{
	return FindManifestIndex(_name) != -1;
}
bool AssetManager::ContainerExists(int _index)
{
	return (_index >= 0) && (_index < this->m_manifests.size());
}



AssetManager::AssetManager(void)
{
	this->CreateContainer(DEFAULT_MANIFEST);
}

AssetManager::~AssetManager(void)
{
}

int AssetManager::CreateContainer(const std::string& _name)
{
	if (!this->ContainerExists(_name))
	{
		size_t index = this->m_manifests.size();
		this->m_manifests.resize(index + 1);
		this->m_manifests[index].m_name = _name;
		return (int)index;
	}
	return -1;
}






int AssetManager::FindAssetIndex(int _manifIndex, const std::string& _name, AssetType _type)
{
	for (int i = ((int)this->m_manifests[_manifIndex].m_assets.size())-1; i >= 0; i--)
	{
		
	}

	return -1;
}

AssetManager::Containers::Containers(void)
{
	this->m_name = "";
}

int AssetManager::Containers::FindAssetIndex(const std::string& _name, AssetType _type)
{
	for (int i = 0; i < this->m_assets.size(); i++)
	{
		Asset& asset = this->m_assets[i];
		if ((asset.name == _name) && ((asset.type == _type) || _type == AssetType::UNKNOWN))
		{
			return i;
		}
	}
	return -1;
}
bool AssetManager::Containers::AssetExists(const std::string& _name, AssetType _type)
{
	return this->FindAssetIndex(_name, _type) != -1;
}

// Asset Manager C++ v1.0