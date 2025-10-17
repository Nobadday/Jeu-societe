#pragma once

#ifndef _INC_ASSET_MANAGER_HPP
#define _INC_ASSET_MANAGER_HPP

#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <any>
#include <map>

#define ASSET_MANAGER_DEFAULT_MANIFEST ""

// File loaded assets Manager
class AssetManager
{
	public:
		enum AssetType
		{
			// Unknown asset type
			UNKNOWN = -1,
			IMAGE,
			TEXTURE,
			FONT,
			SOUND_BUFFER,
			MUSIC,
			
			TEXTURE_ATLAS,
			TEXTURE_ANIMATED,

			ASSET_TYPES
		};
		
		struct Asset
		{
			std::string name;
			AssetType type;
			std::any object;
		};

	public:
		static const char* GetAssetTypeName(AssetType _type);

		AssetManager(void);
		// Destroys the asset manager and destroys all of it's content except if you had unvalid asset types resulting in a memory leak, good job you absolute bafo-
		~AssetManager(void);

		// Creates an empty container
		void CreateManifest(const std::string& _name);
		
		// Loads a manifest, return it's internal index, -1 if failed
		int LoadManifest(const std::string& _filePath, const std::string& _name = nullptr);
		bool UnloadManifest(int _index);
		bool UnloadManifest(const std::string& _name);
		
		// Unload the last manifest loaded;
		bool UnloadLastManifest(void);


		void AddAsset(const std::string& _name, const std::string& _manifest, void* _object, AssetType _type);

		// Add an *already loaded asset* to the asset manager
		// Warning ! You must provide a valid asset type or else it cannot be deleted automatically
		void AddAsset(const std::string& _name, void* _object, AssetType _type);
		
		
		//void AddAsset(const std::string& _name, const std::string& _filePath, AssetType _type);
		
		// Creates and adds an asset loaded from a file
		// If name is null, set as the same value as filepath
		void CreateAsset(const std::string& _name, const std::string& _filePath, AssetType _type);


		// Gets the ID of the first asset found with the given name and type
		// Returns -1 if it wasnt found
		int FindAssetIndex(const std::string& _name, AssetType _type);

		// Returns true if the assets exits
		bool AssetExists(const std::string& _name, AssetType _type);

		

	private:
		// Manifest but also considered as a container
		struct Manifest
		{
			std::string name;
			int startIndex;
			int lenght;
		};
		std::vector<Asset> m_assets;
		std::vector<Manifest> m_manifests;
};


#endif
// Asset Manager C++ v1.0