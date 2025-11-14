#pragma once
#ifndef _INC_ANIMATION_TEXTUREATLAS_HPP
#define _INC_ANIMATION_TEXTUREATLAS_HPP

#include "../Common.hpp"

// C++
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// SFML 2.6.2
#include <SFML/Graphics.hpp>

// Nlohamnn JSON
#include <nlohmann/json.hpp>
// XML
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>

// Yamelo Libs (mine :3 )
#include "../../Utilities/FileTools.hpp"
#include "./AtlasContainer.hpp"



// Texture containing multiple images
// Also known as spritesheet
class TextureAtlas : private sf::Texture, public AtlasContainer
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
		TextureAtlas(void);

		
		// Create the texture
		// If this function fails, the texture & atlas data is left unchanged.
		bool Create(unsigned _width, unsigned _height);


		// Load from file the picture of the texture, 
		// if _resetAtlas is true, will clear all atlas data
		bool LoadFromFilePicture(const sf::String& _filePath, bool _resetAtlas = true, const sf::IntRect& _area = sf::IntRect());
		// Load from file the picture of the texture from an image, 
		// if _resetAtlas is true, will clear all atlas data
		bool LoadFromFilePicture(const sf::Image& _image, bool _resetAtlas = true, const sf::IntRect& _area = sf::IntRect());

		// Load the textureAtlas from a known format, some format may load the texture image along side the atlas data
		bool LoadFromFile(const std::string& _fileName, TextureAtlas::ParseType _parseType);
		bool LoadFromFile(std::fstream& _file, const std::string& _dirPath, TextureAtlas::ParseType _parseType);


		// Find the index of the first frame that contains size _name
		// _name = "Idle"
		// Compare (_name with Idle0000 (truncate to the size of _name)), Idle == Idle ?
		int FindFrameGroupIndex(const std::string& _name);
		int FindFrameGroupLenght(const std::string& _name, int _startIndex = 0);
		int FindFrameGroupIndexLast(const std::string& _name);
		AtlasFrame& GetFrameByGroup(const std::string& _name, int _index);


		using sf::Texture::getSize;
		using sf::Texture::copyToImage;
		using sf::Texture::setSmooth;
		using sf::Texture::isSmooth;
		using sf::Texture::setSrgb;
		using sf::Texture::isSrgb;
		using sf::Texture::setRepeated;
		using sf::Texture::isRepeated;


		const sf::Texture* GetTexture(void) const;
		const sf::Texture& GetTextureRef(void) const;
		operator const sf::Texture*() const;
		operator const sf::Texture&() const;

		
};

#endif

// Texture Atlases for C++ & SFML v1.1.2