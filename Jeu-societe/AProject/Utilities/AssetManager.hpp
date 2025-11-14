#pragma once

#ifndef _INC_ASSETMANAGER_HPP
#define _INC_ASSETMANAGER_HPP

#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include "../Animation/Graphics/TextureAnimated.hpp"


// File loaded-assets Manager
class AssetManager
{
	public:
		enum AssetType
		{
			// Unknown asset type
			UNKNOWN = -1,

			IMAGE = 0,
			TEXTURE,
			FONT,
			SOUND_BUFFER,
			MUSIC,

			TEXTURE_ATLAS,
			TEXTURE_ANIMATED,

			// Count of all available types
			ASSET_TYPES
		};
		
		struct Asset
		{
			std::string name;
			AssetType type = AssetType::UNKNOWN;
			std::shared_ptr<void> object;
		};

	public:
		static bool AssetTypeIsValid(AssetType _type);
		static const char* GetAssetTypeName(AssetType _type);
		// Get the default placeholder for the given asset type
		static const char* GetAssetTypePlaceholder(AssetType _type);
		static AssetType GetAssetTypeFromName(const std::string& _name);

	private:
		class Container
		{
			public:
				std::string m_name;
				std::vector<Asset> m_assets;

			public:
				Container(void);
				~Container(void);

				template <typename T>
				void AddAsset(const std::string& _name, T* _object, AssetType _type = AssetType::UNKNOWN);

				// TODO :
				//void AddAsset(const std::string& _name, std::shared_ptr<void> _object, AssetType _type = AssetType::UNKNOWN);

				int FindAssetIndex(const std::string& _name, AssetType _type);
				bool AssetExists(const std::string& _name, AssetType _type);

				Asset& GetAssetWrap(int _index);
				void* GetAsset(int _index);
				void* GetAsset(const std::string& _name, AssetType _type);

				
				void DeleteAsset(int _index);
				void Clear(void);

		};

	private:
		// Will delete every container and asset
		void Clear(bool _secured);

		// Unloads and destroys a container
		// If secured, doesn't destroy the main container
		void DeleteContainer(int _index, bool _secured);
		

	public:
		AssetManager(void);
		// Destroys the asset manager and destroys all of it's content
		~AssetManager(void);

		// Will delete every container and asset
		void Clear(void);

		// Adds an empty asset container, returns it's index
		int CreateContainer(const std::string& _name);

		void DeleteContainer(int _index);
		void DeleteContainer(const std::string& _name);
		// Delete the last container loaded;
		void DeleteLastContainer(void);


		int FindContainerIndex(const std::string& _name);
		bool ContainerExists(const std::string& _name);
		bool ContainerExists(int _index);
		

		// Loads a manifest from a file and creates a container, return it's internal index, -1 if failed
		// if name == nullptr, it's name will be it's filepath
		bool LoadManifest(const std::string& _filePath, const std::string& _containerName);
		

		// Add an *already loaded asset* to one of the containers of the asset manager
		// manager.AddAsset<sf::Texture>("MyTexture", &myTextureObject, AssetType::TEXTURE);
		template <typename T>
		void AddAsset(const std::string& _name, T* _object, AssetType _type, int _containerID = 0);


		// Returns true if the assets exits
		bool AssetExists(const std::string& _name, AssetType _type);


		Container& GetContainer(int _index);
		Container& GetContainer(const std::string& _name);


		// Get any asset associated with this name from last to first container
		void* GetAsset(const std::string& _name, AssetType _type = AssetType::UNKNOWN);

		// Get an asset associated with this name from last to first container,
		// If the asset is not found, tries to get the default placeholder asset
		void* GetAsset(const std::string& _name, AssetType _type, const std::string& _placeholder);

		// Get an asset associated with this name from last to first container,
		// If the asset is not found, tries to get the default placeholder available for this type
		void* GetAssetOrPlaceholder(const std::string& _name, AssetType _type);

		// Get an asset associated with this name from last to first container,
		// If the asset is not found, gets the default placeholder available for this type
		template <typename T>
		T* GetAsset(const std::string& _name, AssetType _type = AssetType::UNKNOWN);

	private:
		std::vector<Container> m_containers;
};


template<typename T>
inline void AssetManager::Container::AddAsset(const std::string& _name, T* _object, AssetType _type)
{
	int index = this->FindAssetIndex(_name, _type);

	if (index != -1)
	{
		printf("[WARNING] Asset Manager : Name conflict with \"%s\" of type \"%s\". Will destroy the previous object...\n", _name.c_str(), AssetManager::GetAssetTypeName(_type));
		this->DeleteAsset(index);
		// TODO : Do not delete the asset wrapper, just replace the contained object...
	}
	
	index = (int)this->m_assets.size();
	this->m_assets.resize(index + 1);
	Asset& asset = this->m_assets[index];
	asset.name = _name;
	asset.type = _type;
	std::shared_ptr<T> testShared(_object);
	asset.object = testShared;
}


template<typename T>
inline void AssetManager::AddAsset(const std::string& _name, T* _object, AssetType _type, int _containerID)
{
	this->GetContainer(_containerID).AddAsset<T>(_name, _object, _type);
}

template<typename T>
inline T* AssetManager::GetAsset(const std::string& _name, AssetType _type)
{
	return (T*)this->GetAssetOrPlaceholder(_name, _type);
}

#endif
// Asset Manager C++ v1.0.3