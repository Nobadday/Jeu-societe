#pragma once

#ifndef _INC_ANIMATION_TEXTURE_ATLAS_HPP
#define _INC_ANIMATION_TEXTURE_ATLAS_HPP

#include "../Common.hpp"

#include <string>
#include <fstream>
// SFML 2.6.2
#include <SFML/Graphics.hpp>
// Nlohamnn JSON
#include <nlohmann/json.hpp>
// Yamelo Libs (me :3 )
#include "../../Utilities/Lists/DynaList.hpp"
#include "../../Utilities/FileTools.hpp"
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>
#include <iostream>

class AtlasFrame
{
	public:
		// Values with associated booleans are optional parameters, if bool is true than the variable is initialised

		std::string m_name;
		// Subtexture rect, where to pick the image on the texture
		sf::IntRect m_textureRect;
		
		// Frame offset, x & y should be added
		// w & h indicate the in game size of the frame
		sf::IntRect m_frameOffset;

		bool m_flipX = false;
		bool m_flipY = false;
		// Is rect rotated by 90° ?
		bool m_rotated = false;

		bool m_hasTilt = false;
		// Additional tilt that should be applied
		sf::Vector2f m_frameTilt;

	public:
		AtlasFrame(void);
		
		void SetTextureRect(int _x, int _y, int _w, int _h);

};

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
};

#endif

// Texture Atlases for C++ & SFML v1.0