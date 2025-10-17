#pragma once

#ifndef _INC_ASSET_MANAGER_HPP
#define _INC_ASSET_MANAGER_HPP

#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <any>


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

	private:
		// Creates an empty asset container
		int CreateContainer(const std::string& _name);
		

		int FindManifestIndex(const std::string& _name);
		

		bool UnloadManifest(int _index, bool _secured);

	public:
		static const char* GetAssetTypeName(AssetType _type);

		AssetManager(void);
		// Destroys the asset manager and destroys all of it's content except if you had unvalid asset types resulting in a memory leak, good job you absolute bafo-
		~AssetManager(void);

		bool ContainerExists(const std::string& _name);
		bool ContainerExists(int _index);

		// Loads a manifest from a file, return it's internal index, -1 if failed
		// if name == nullptr, it's name will be it's filepath
		int LoadManifest(const std::string& _filePath, const std::string& _name = nullptr);
		// Unloads a manifest
		bool UnloadManifest(int _index);
		bool UnloadManifest(const std::string& _name);
		
		// Unload the last manifest loaded;
		bool UnloadLastManifest(void);


		void AddAsset(const std::string& _name, void* _object, AssetType _type, int _manifestID);
		void AddAsset(const std::string& _name, void* _object, AssetType _type, const std::string& _manifest);

		// Add an *already loaded asset* to the asset manager
		// Warning ! You must provide a valid asset type or else it cannot be deleted automatically
		void AddAsset(const std::string& _name, void* _object, AssetType _type);
		
		
		//void AddAsset(const std::string& _name, const std::string& _filePath, AssetType _type);
		
		// Creates and adds an asset loaded from a file
		// If name is null, set as the same value as filepath
		void CreateAsset(const std::string& _name, const std::string& _filePath, AssetType _type, int _containerID);
		
		// <Temp : GLOBAL>
		void CreateAsset(const std::string& _name, const std::string& _filePath, AssetType _type);

		int FindAssetIndex(int _manifIndex, const std::string& _name, AssetType _type);

		// Gets the ID of the first asset found with the given name and type
		// Returns -1 if it wasnt found
		int FindAssetIndex(const std::string& _name, AssetType _type);

		// Returns true if the assets exits
		bool AssetExists(const std::string& _name, AssetType _type);


		void* GetAsset(int _manifIndex, int _assetIndex, AssetType _type);


	private:
		class Containers
		{
			public:
				std::string m_name = "";
				std::vector<Asset> m_assets;
			
			public:
				Containers(void);
				int FindAssetIndex(const std::string& _name, AssetType _type);
				bool AssetExists(const std::string& _name, AssetType _type);
				template <typename Type>
				Type GetAsset(int _index, AssetType _type);
				void* GetAsset(const std::string& _name, AssetType _type);
				

		};
		std::vector<Containers> m_manifests;
		//std::vector<Asset> m_assets;
};


#endif
// Asset Manager C++ v1.0