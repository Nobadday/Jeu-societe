#pragma once
#ifndef _INC_ANIMATION_TEXTUREANIMATED_HPP
#define _INC_ANIMATION_TEXTUREANIMATED_HPP


#include "../Common.hpp"
#include "TextureAtlas.hpp"
#include "../Animation/AnimationHandler.hpp"
#include "../Utilities/Math.hpp"
#include <string>



// Data for spritesheet animations
// contains all animations details like
// framerate, looping etc...
class TextureAnimated
{
	public:
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
			
			public:
				AnimationProperties(void);

		};

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

	private:
		TextureAtlas m_textureAtlas;
		std::vector<AnimationProperties> m_animations;

	public:
		TextureAnimated(void);
		~TextureAnimated(void);
		

		// Load an animation file of any of the supported formats given with _animationFormat
		bool LoadFromFile(const std::string& _fileName, TextureAnimated::AnimationType _animationFormat);
		// Load an animation file of any of the supported formats given with _animationFormat
		bool LoadFromFile(std::fstream& _file, const std::string& _dirPath, TextureAnimated::AnimationType _animationFormat);

		// [INTENT TO BE DEPRECATED]
		// Load only a texture atlas and creates an animation of the entire atlas
		bool LoadFromFile(const std::string& _fileName, TextureAtlas::ParseType _atlasParseType);
		// [INTENT TO BE DEPRECATED]
		// Load only a texture atlas and creates an animation of the entire atlas
		bool LoadFromFile(std::fstream& _file, const std::string& _dirPath, TextureAtlas::ParseType _atlasParseType);
		

		//void AddAnimation(const std::string& _name, const std::vector<int>& _frameArrays, bool _loop = false, float _framerate = 24.0f);
		//void AddAnimation(const std::string& _name, const std::vector<std::string>& _frameNames, bool _loop, float _framerate);

		AnimationProperties& GetAnimation(int _index);
		AnimationProperties& GetAnimation(const std::string& _name);
		// -1 if not found
		int FindAnimationIndex(const std::string& _name);
		bool AnimationExists(const std::string& _name);
		bool AnimationExists(int _index);

		AtlasFrame& GetFrame(int _atlasIndex, int _frame);

		const TextureAtlas& GetTextureAtlas(void);
		const sf::Texture& GetTexture(void);


		size_t GetAnimationCount(void);
	
	protected:
		void ClearAnimations(void);
};


#endif
// Texture Animated v1.2.3
// TODO : Remake to be accessible everywhere, ability to AddAnimations like AxeFlixel