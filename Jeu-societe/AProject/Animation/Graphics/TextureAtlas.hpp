#pragma once
#ifndef _INC_ANIMATION_TEXTUREATLAS_HPP
#define _INC_ANIMATION_TEXTUREATLAS_HPP

#include "../Common.hpp"

// C++
#include <iostream>
#include <string>
#include <fstream>
//#include <vector>

// SFML 2.6.2
#include <SFML/Graphics.hpp>

// Nlohamnn JSON
#include <nlohmann/json.hpp>

// XML
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>

// Yamelo Libs (me :3 )
#include "../../Utilities/Lists/DynaList.hpp"
#include "../../Utilities/FileTools.hpp"
#include "./AtlasContainer.hpp"




class AtlasFrameGroup
{
	public:
		std::string m_baseName;
		int m_index;
		int m_lenght;
};

// Texture containing multiple images in it
// Also known as spritesheet
class TextureAtlas
{
	protected:
		sf::Texture m_texture;
		// List of AtlasFrame*
		DynaList m_frameList;

	public:
		enum ParseType
		{
			// Sparrow v2
			// Mostly used for Adobe Animate Spritesheet exports
			// https://enigmaengine.github.io/docs/animation-format/
			PARSE_XML_SPARROW_V2,


			// Detects if json is either in HASH or ARRAY
			// Not recommended
			PARSE_JSON_AUTO,

			// JSON format
			PARSE_JSON_HASH,

			// JSON-Array format
			// Compatible with Aseprite & Animate
			PARSE_JSON_ARRAY,
			
			
			PARSETYPES_COUNT
		};

	public:
		// Default constructor, creates an empty texture
		TextureAtlas(void);

		~TextureAtlas(void);


		// Load the textureAtlas from a known format
		bool LoadFromFile(const std::string& _fileName, TextureAtlas::ParseType _parseType);
		bool LoadFromFile(std::fstream& _file, const std::string& _dirPath, TextureAtlas::ParseType _parseType);

		AtlasFrame* GetFrame(int _index);
		// Returns the pointer of a frame, identify with EXACT name
		AtlasFrame* GetFrame(const std::string& _name);
		


		int GetFrameCount(void);

		// Get the index of a frame
		// If not found, return -1
		int FindFrameIndex(const std::string& _name);

		bool FrameExists(const std::string& _name);

		const sf::Texture* GetTexture(void);
		const sf::Texture& GetTextureRef(void);

		void ClearAll(void);

		// Find the index of the first frame that contains size _name
		// _name = "Idle"
		// Compare (_name with Idle0000 (truncate to the size of _name)), Idle == Idle ?
		int FindFrameGroupIndex(const std::string& _name);

		int FindFrameGroupLenght(const std::string& _name, int _startIndex = 0);

		int FindFrameGroupIndexLast(const std::string& _name);

		AtlasFrame* GetFrameByGroup(const std::string& _name, int _index);
	
	private:


};

// Texture containing multiple images
// Also known as spritesheet
class TextureWhat : public sf::Texture, public AtlasContainer
{
	public:
		enum ParseType
		{
			// Sparrow v2
			// Mostly used for Adobe Animate Spritesheet exports
			// https://enigmaengine.github.io/docs/animation-format/
			PARSE_XML_SPARROW_V2,

			// Detects if json is either in HASH or ARRAY
			// Not recommended
			PARSE_JSON_AUTO,

			// JSON format
			PARSE_JSON_HASH,

			// JSON-Array format
			// Compatible with Aseprite & Animate
			PARSE_JSON_ARRAY,


			// Counter of all parse types
			PARSETYPES_COUNT
		};

	public:
		// Default constructor, creates an empty texture
		TextureWhat(void);

		// Create the texture
		// If this function fails, the texture & atlas data is left unchanged.
		bool create(unsigned _width, unsigned _height);


		// Load from file the picture of the texture, 
		// if _resetAtlas is true, will clear all atlas data
		bool LoadFromFilePicture(const sf::String& _filePath, bool _resetAtlas = true, const sf::IntRect& _area = sf::IntRect());
		// Load from file the picture of the texture from an image, 
		// if _resetAtlas is true, will clear all atlas data
		bool LoadFromFilePicture(const sf::Image& _image, bool _resetAtlas = true, const sf::IntRect& _area = sf::IntRect());

		// Load the textureAtlas from a known format, some format may load the texture image along side the atlas data
		bool LoadFromFile(const std::string& _fileName, TextureWhat::ParseType _parseType);
		

	protected:
		bool LoadFromFile(std::fstream& _file, const std::string& _dirPath, TextureWhat::ParseType _parseType);

		using sf::Texture::loadFromFile;
		using sf::Texture::loadFromImage;

};

#endif

// Texture Atlases for C++ & SFML v1.0.2