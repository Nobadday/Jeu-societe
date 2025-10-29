#pragma once

#ifndef _INC_ANIMATION_TEXTURE_HPP
#define _INC_ANIMATION_TEXTURE_HPP

#include "../Common.hpp"
#include "TextureAtlas.hpp"
#include "../Animation/AnimationHandler.hpp"
#include "../Utilities/Math.hpp"
#include <string>

#define ANIM_DEFAULT_FPS 24.0f
#define ANIM_DEFAULT_LOOP false

class AnimationProperties
{
	public:
		std::string m_name;
		std::string m_atlasName;
		int m_atlasIndex;
		int m_frameCount;
		float m_framerate;
		bool m_loop;
		sf::Vector2f m_offset;
		
		AnimationProperties(void);
};

// Data for spritesheet animations
// contains all animations details like
// framerate, looping etc...
class TextureAnimated
{
	public:
		enum AnimationType
		{
			// Old animation format :
			// .texanim
			ANIMATION_TEXANIM,

			// New animation format
			ANIMATION_ANIM,

			// Animation files produced by Aseprite,
			// only available if you exported your animation
			// with tags meta
			ANIMATION_ASEPRITE,

			// Animation files of Friday Night Funckin' (Legacy)
			ANIMATION_FNF_LEGACY,

			// Animation files of Friday Night Funckin'
			ANIMATION_FNF,

			ANIMATIONTYPES_COUNT
		};
		TextureAnimated(void);
		~TextureAnimated(void);
		
		bool LoadFromFile(const std::string& _fileName, TextureAnimated::AnimationType _animationFormat);
		bool LoadFromFile(std::fstream& _file, const std::string& _dirPath, TextureAnimated::AnimationType _animationFormat);

		// Load only a texture atlas and creates an animation of the entire atlas
		bool LoadFromFile(const std::string& _fileName, TextureAtlas::ParseType _atlasParseType);
		// Load only a texture atlas and creates an animation of the entire atlas
		bool LoadFromFile(std::fstream& _file, const std::string& _dirPath, TextureAtlas::ParseType _atlasParseType);

		void ClearAll(void);

		AnimationProperties* GetAnimation(int _index);
		AnimationProperties* GetAnimation(const std::string& _name);
		// -1 if not found
		int FindAnimationIndex(const std::string& _name);
		bool AnimationExists(const std::string& _name);
		bool AnimationExists(int _index);

		AtlasFrame* GetFrame(int _atlasIndex, int _frame);

		const sf::Texture* GetTexture(void);
		const sf::Texture& GetTextureRef(void);

		int GetAnimationCount(void);

	private:
		TextureAtlas m_textureAtlas;
		// List of AnimationProperties*
		DynaList m_animations;
};

#endif
// Texture Animated v1.0