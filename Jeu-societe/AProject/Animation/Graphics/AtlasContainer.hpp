#pragma once
#ifndef _INC_ATLASCONTAINER_HPP
#define _INC_ATLASCONTAINER_HPP


#include "../Common.hpp"
#include <SFML/Graphics.hpp>


class AtlasFrame
{
	public:
		std::string m_name;
		// Subtexture rect, where to pick the image on the texture
		sf::IntRect m_textureRect;

		// Frame offset, x & y should be added
		// w & h indicate the in game size of the frame
		sf::IntRect m_frameOffset;

		bool m_flipX;
		bool m_flipY;
		// Is rect rotated by 90° ?
		bool m_rotated;

		bool m_hasTilt;
		// Additional tilt that should be applied
		sf::Vector2f m_frameTilt;

	public:
		AtlasFrame(void);

		void SetTextureRect(int _x, int _y, int _w, int _h);

};


class AtlasContainer
{
	protected:
		std::vector<AtlasFrame> m_frames;

	public:
		AtlasContainer(void);


		// Appens an atlas frame into the container and gets it's reference
		// If it already exists, gets that reference
		AtlasFrame& ModifyAtlasFrame(const std::string& _name);


		// Get the index of a atlas frame
		// If not found, return -1
		int FindAtlasFrameIndex(const std::string& _name);
		bool AtlasFrameExists(const std::string& _name);

		AtlasFrame& GetAtlasFrame(int _index);
		AtlasFrame& GetAtlasFrame(const std::string& _name);

		size_t GetAtlasFrameCount(void);
	
	protected:
		void ClearAtlasFrames(void);
};


#endif
// AtlasContainer C++ || v0.2