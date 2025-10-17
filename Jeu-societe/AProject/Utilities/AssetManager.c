#include "AssetManager.h"


typedef struct Asset
{
	char* name;
	AssetType type;
	void* content;
} Asset;

typedef struct AssetManager
{
	PyList* contents;
} AssetManager;



Asset* AssetCreate(char* _name, void* _content, AssetType _type)
{
	Asset* ptr = calloc((size_t)1, sizeof(Asset));
	if (ptr != NULL)
	{
		ptr->content = _content;
		ptr->type = _type;
		size_t size = strlen(_name)+1 * sizeof(char);
		ptr->name = malloc(size);
		if (ptr->name == NULL)
		{
			ptr->name = "";
		}
		else
		{
			strcpy_s(ptr->name, size, _name);
		}
		
		//ptr->name = _name;
	}
	return ptr;
}

void* AssetGetContent(Asset* _asset)
{
	return _asset->content;
}

void AssetDestroy(Asset* _asset)
{
	free(_asset->name);
	free(_asset);
}



AssetManager* AssetManagerCreate(void)
{
	AssetManager* ptr = calloc((size_t)1, sizeof(AssetManager));
	if (ptr != NULL)
	{
		ptr->contents = PyListCreate();
	}
	return ptr;
}

void AssetManagerLoadManifest(AssetManager* _manager, char* _path)
{
	ASSET_MANAGER_DEBUG_PRINTF("[INFO] Asset Manager : Loading manifest '%s'\n", _path);
	cJSON* json = cJSON_CreateFromFile(_path);
	if (json != NULL)
	{
		cJSON* array = cJSON_GetObjectItem(json, "assets");
		cJSON* arrayItem;

		char* assetName;
		char* assetPath;
		AssetType type;

		for (int i = 0; i < cJSON_GetArraySize(array); i++)
		{
			arrayItem = cJSON_GetArrayItem(array, i);
			
			assetPath = cJSON_GetObjectItem(arrayItem, "path")->valuestring;
			assetName = cJSON_GetObjectItem(arrayItem, "name")->valuestring;
			type = cJSON_GetObjectItem(arrayItem, "type")->valueint;

			AssetManagerAssetCreate(_manager, type, assetName, assetPath);
		}

		cJSON_Destroy(json);
	}
	else
	{
		printf("[WARNING] Asset Manager : Failed to load manifest file : '%s'\n", _path);
	}
	
}

void AssetManagerUnloadManifest(AssetManager* _manager, char* _path)
{
	ASSET_MANAGER_DEBUG_PRINTF("[INFO] Asset Manager : Unloading manifest '%s'\n", _path);
	cJSON* json = cJSON_CreateFromFile(_path);
	if (json != NULL)
	{
		cJSON* array = cJSON_GetObjectItem(json, "assets");
		cJSON* arrayItem;

		char* assetName;
		AssetType type;

		for (int i = 0; i < cJSON_GetArraySize(array); i++)
		{
			arrayItem = cJSON_GetArrayItem(array, i);

			assetName = cJSON_GetObjectItem(arrayItem, "name")->valuestring;
			type = cJSON_GetObjectItem(arrayItem, "type")->valueint;

			AssetManagerDestroyAssetTarget(_manager, type, assetName);
		}


		cJSON_Destroy(json);
	}
	else
	{
		printf("[WARNING] Asset Manager : Failed to load manifest file : '%s'\n", _path);
	}
}

void AssetManagerAssetAdd(AssetManager* _manager, AssetType _type, char* _name, void* _content)
{
	if (_name == NULL)
	{
		_name = "Unnamed";
	}
	if (AssetManagerAssetExists(_manager, _type, _name))
	{
		
		ASSET_MANAGER_DEBUG_PRINTF("[WARNING] Asset Manager : Attempted to add an asset already created with name '%s' and type (%d). Will attempt to destroy it...\n", _name, _type);
		AssetManagerDestroyAssetTarget(_manager, _type, _name);
	}
	PyListAppend(_manager->contents, AssetCreate(_name, _content, _type));
}

void AssetManagerAssetCreate(AssetManager* _manager, AssetType _type, char* _name, const char* _filePath)
{
	if (_name == NULL)
	{
		_name = _filePath;
	}
	void* ptr = NULL;
	switch (_type)
	{
		case ASSET_TYPE_TEXTURE:
			ptr = sfTexture_createFromFile(_filePath, NULL);
			break;
		case ASSET_TYPE_IMAGE:
			ptr = sfImage_createFromFile(_filePath);
			break;
		case ASSET_TYPE_SOUNDBUFFER:
			ptr = sfSoundBuffer_createFromFile(_filePath);
			break;
		case ASSET_TYPE_MUSIC:
			ptr = sfMusic_createFromFile(_filePath);
			break;
		case ASSET_TYPE_FONT:
			ptr = sfFont_createFromFile(_filePath);
			break;
		case ASSET_TYPE_TEXTURE_ANIMATED:
			ptr = TextureAnimatedCreateFromFile(_filePath);
			break;

		case ASSET_TYPE_ANY:
		default:
			// Unknown type !!!
			ASSET_MANAGER_DEBUG_PRINTF("[WARNING] Asset Manager : Created a NULL asset with unknown type %s\n", _name);
			_type = ASSET_TYPE_ANY;
			break;
	}
	if (ptr == NULL)
	{
		ASSET_MANAGER_DEBUG_PRINTF("[WARNING] Asset Manager : Failed to load asset's ressource with key : '%s' (returned NULL) || Type : '%d' || Path : '%s'\n", _name, _type, _filePath);
	}
	AssetManagerAssetAdd(_manager, _type, _name, ptr);
}

int AssetManagerFindAssetID(AssetManager* _manager, AssetType _type, char* _name)
{
	Asset* assetPTR;
	for (int i = 0; i < PyListGetLen(_manager->contents); i++)
	{
		assetPTR = PyListGetElement(_manager->contents, i);
		if ((assetPTR->type == _type) && (strcmp(assetPTR->name, _name) == 0))
		{
			return i;
		}
	}
	return -1;
}

sfBool AssetManagerAssetExists(AssetManager* _manager, AssetType _type, char* _name)
{
	return AssetManagerFindAssetID(_manager, _type, _name) != -1;
}

void* AssetManagerGetAssetContent(AssetManager* _manager, AssetType _type, char* _name)
{
	int assetID = AssetManagerFindAssetID(_manager, _type, _name);
	if (assetID != -1)
	{
		return AssetGetContent(PyListGetElement(_manager->contents, assetID));
	}
	return NULL;
}

void* AssetManagerGetAssetContentOrPlaceholder(AssetManager* _manager, AssetType _type, char* _name, char* _placeholderName)
{
	void* content = AssetManagerGetAssetContent(_manager, _type, _name);
	if (content == NULL)
	{
		content = AssetManagerGetAssetContent(_manager, _type, _placeholderName);
	}
	return content;
}

sfTexture* AssetManagerGetTexture(AssetManager* _manager, char* _name)
{
	return AssetManagerGetAssetContentOrPlaceholder(_manager, ASSET_TYPE_TEXTURE, _name, ASSET_MANAGER_PLACEHOLDER_TEXTURE);
}

sfImage* AssetManagerGetImage(AssetManager* _manager, char* _name)
{
	return AssetManagerGetAssetContentOrPlaceholder(_manager, ASSET_TYPE_IMAGE, _name, ASSET_MANAGER_PLACEHOLDER_IMAGE);
}

sfSoundBuffer* AssetManagerGetSoundBuffer(AssetManager* _manager, char* _name)
{
	return AssetManagerGetAssetContentOrPlaceholder(_manager, ASSET_TYPE_SOUNDBUFFER, _name, ASSET_MANAGER_PLACEHOLDER_SOUNDBUFFER);
}

sfMusic* AssetManagerGetMusic(AssetManager* _manager, char* _name)
{
	return AssetManagerGetAssetContentOrPlaceholder(_manager, ASSET_TYPE_MUSIC, _name, ASSET_MANAGER_PLACEHOLDER_MUSIC);
}

sfFont* AssetManagerGetFont(AssetManager* _manager, char* _name)
{
	return AssetManagerGetAssetContentOrPlaceholder(_manager, ASSET_TYPE_FONT, _name, ASSET_MANAGER_PLACEHOLDER_FONT);
}

TextureAnimated* AssetManagerGetTextureAnimated(AssetManager* _manager, char* _name)
{
	return AssetManagerGetAssetContentOrPlaceholder(_manager, ASSET_TYPE_TEXTURE_ANIMATED, _name, ASSET_MANAGER_PLACEHOLDER_TEXTUREANIMATED);
}

void AssetManagerDestroyAsset(AssetManager* _manager, int _index)
{
	Asset* assetPTR = PyListPop(_manager->contents, _index);
	switch (assetPTR->type)
	{
		case ASSET_TYPE_TEXTURE:
			sfTexture_destroy(assetPTR->content);
			break;
		case ASSET_TYPE_IMAGE:
			sfImage_destroy(assetPTR->content);
			break;
		case ASSET_TYPE_SOUNDBUFFER:
			sfSoundBuffer_destroy(assetPTR->content);
			break;
		case ASSET_TYPE_MUSIC:
			sfMusic_destroy(assetPTR->content);
			break;
		case ASSET_TYPE_FONT:
			sfFont_destroy(assetPTR->content);
			break;
		case ASSET_TYPE_TEXTURE_ANIMATED:
			TextureAnimatedDestroy(assetPTR->content);
			break;

		case ASSET_TYPE_ANY:
		default:
			// Unknown type !!!
			ASSET_MANAGER_DEBUG_PRINTF("[WARNING] Asset Manager : Couldn't deallocate asset of an unknown type !\n");
			
			break;
	}
	AssetDestroy(assetPTR);
}

void AssetManagerDestroyAssetTarget(AssetManager* _manager, AssetType _type, char* _name)
{
	int assetID = AssetManagerFindAssetID(_manager, _type, _name);
	if (assetID != -1)
	{
		AssetManagerDestroyAsset(_manager, assetID);
	}
	else
	{
		ASSET_MANAGER_DEBUG_PRINTF("[WARNING] Asset Manager : Couldn't destroy the asset '%s' of type %d !\n", _name, _type);
	}
}

void AssetManagerDestroyAssetAll(AssetManager* _manager)
{
	for (int i = PyListGetLen(_manager->contents) - 1; i >= 0; i--)
	{
		AssetManagerDestroyAsset(_manager, i);
	}
}

void AssetManagerDestroy(AssetManager* _manager)
{
	AssetManagerDestroyAssetAll(_manager);
	PyListDestroy(_manager->contents);
	free(_manager);
}


// Asset Manager v1.5