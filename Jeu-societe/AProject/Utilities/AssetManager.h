#pragma once

#ifndef _INC_ASSET_MANAGER
#define _INC_ASSET_MANAGER

// C Libs
#ifndef _INC_MALLOC
#include <malloc.h>
#endif
#ifndef _INC_STRING
#include <string.h>
#endif
// CSFML
#ifndef SFML_GRAPHICS_H
#include <SFML/Graphics.h>
#endif
#ifndef SFML_AUDIO_H
#include <SFML/Audio.h>
#endif


#ifndef _INC_TEXTURE_ANIMATED
#include "../Animation/TextureAnimated.h"
#endif

#ifndef cJSONAddons__h
#include "./cJSON/cJSONAddons.h"
#endif


#ifndef _INC_PYLIST
#include "./Lists/PyList.h"
#endif


#define ASSET_MANAGER_PLACEHOLDER_TEXTURE "SYSTEM:Placeholder"
#define ASSET_MANAGER_PLACEHOLDER_IMAGE "SYSTEM:Placeholder:Image"
#define ASSET_MANAGER_PLACEHOLDER_SOUNDBUFFER "SYSTEM:Placeholder:Sound"
#define ASSET_MANAGER_PLACEHOLDER_MUSIC "SYSTEM:Placeholder:Music"
#define ASSET_MANAGER_PLACEHOLDER_FONT "SYSTEM:Placeholder:Font"
#define ASSET_MANAGER_PLACEHOLDER_TEXTUREANIMATED "SYSTEM:Placeholder:Animated"


#if 1
#define ASSET_MANAGER_DEBUG_PRINTF printf
#else
#define ASSET_MANAGER_DEBUG_PRINTF //
#endif


typedef enum AssetType
{
	ASSET_TYPE_ANY = 0,
	ASSET_TYPE_TEXTURE,
	ASSET_TYPE_IMAGE,
	ASSET_TYPE_SOUNDBUFFER,
	ASSET_TYPE_MUSIC,
	ASSET_TYPE_FONT,
	ASSET_TYPE_TEXTURE_ANIMATED,

	ASSET_TYPES_COUNT
} AssetType;

typedef struct Asset Asset;

typedef struct AssetManager AssetManager;


Asset* AssetCreate(char* _name, void* _content, AssetType _type);
void* AssetGetContent(Asset* _asset);
void AssetDestroy(Asset* _asset);


AssetManager* AssetManagerCreate(void);

void AssetManagerLoadManifest(AssetManager* _manager, char* _path);
void AssetManagerUnloadManifest(AssetManager* _manager, char* _path);

// Add an *already loaded asset* to the asset manager
// Warning ! You must provide a valid asset type or else it cannot be deleted autimatically
void AssetManagerAssetAdd(AssetManager* _manager, AssetType _type, char* _name, void* _content);

// Creates and adds an asset loaded from a file
// If name is null, set as the same value as filepath
void AssetManagerAssetCreate(AssetManager* _manager, AssetType _type, char* _name, const char* _filePath);

// Gets the ID of the first asset found with the given name and type
// Returns -1 if it wasnt found
int AssetManagerFindAssetID(AssetManager* _manager, AssetType _type, char* _name);

// Returns true if the assets exits
sfBool AssetManagerAssetExists(AssetManager* _manager, AssetType _type, char* _name);

// Get the first asset with the given type and name
void* AssetManagerGetAssetContent(AssetManager* _manager, AssetType _type, char* _name);
void* AssetManagerGetAssetContentOrPlaceholder(AssetManager* _manager, AssetType _type, char* _name, char* _placeholderName);
sfTexture* AssetManagerGetTexture(AssetManager* _manager, char* _name);
sfImage* AssetManagerGetImage(AssetManager* _manager, char* _name);
sfSoundBuffer* AssetManagerGetSoundBuffer(AssetManager* _manager, char* _name);
sfMusic* AssetManagerGetMusic(AssetManager* _manager, char* _name);
sfFont* AssetManagerGetFont(AssetManager* _manager, char* _name);
TextureAnimated* AssetManagerGetTextureAnimated(AssetManager* _manager, char* _name);

// Destroys the asset and destroys the contents of it
// Warning if given an invalid index : result in unexpected beheaviors
void AssetManagerDestroyAsset(AssetManager* _manager, int _index);
void AssetManagerDestroyAssetTarget(AssetManager* _manager, AssetType _type, char* _name);
void AssetManagerDestroyAssetAll(AssetManager* _manager);

// Destroys the asset manager and destroys all of it's content except if you had unvalid asset types resulting in a memory leak, good job you absolute bafo-
void AssetManagerDestroy(AssetManager* _manager);

#endif
// Asset Manager v1.5