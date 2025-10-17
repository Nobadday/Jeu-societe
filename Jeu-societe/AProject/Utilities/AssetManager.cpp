#include "AssetManager.hpp"

#ifdef ASSET_MANAGER_DEBUG
#define DEBUG_PRINT printf
#else
#define DEBUG_PRINT //
#endif


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


AssetManager::AssetManager(void)
{
	
}

AssetManager::~AssetManager(void)
{
}


// Asset Manager C++ v1.0