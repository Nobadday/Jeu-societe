#pragma once

#include <SFML/Graphics.hpp>
#include <iostream> // DEBUG REMOVE AFTER TODO &²

// Before compiling, let's first see a quick message
// for my programming school :

// <SchoolName>, PLEASE DON'T KILL ME I-IM JUST
// JUST COPYING SFML'S PROGRAMMING STYLE !!! :(
// I KNOW OUR STYLE BY HEART I PROMISE !!!§1&11!§§

namespace sfMod
{
	class TransformAddle
	{
		private:
			sf::Vector2f          m_shear;
			bool                  m_flipX;
			bool                  m_flipY;
			bool                  m_mirrorX;
			bool                  m_mirrorY;
			sf::Vector2f          m_offset;

			mutable sf::Transform m_transform;					//!< Combined transformation of the object
			mutable bool          m_transformNeedUpdate;		//!< Does the transform need to be recomputed?
			mutable sf::Transform m_inverseTransform;           //!< Combined transformation of the object
			mutable bool          m_inverseTransformNeedUpdate; //!< Does the transform need to be recomputed?
			sf::Transformable* m_test;
		public:
			TransformAddle(sf::Transformable* _test);
			virtual ~TransformAddle();

			void setShear(float _kX, float _kY);

			void setFlip(bool _flipX, bool _flipY);

			void setMirror(bool _mirrorX, bool _mirrorY);
			void setMirrorX(bool _condition);
			void setMirrorY(bool _condition);

			void setOffset(float _offsetX, float _offsetY);

			////////////////////////////////////////////
			void shear(float _kX, float _kY);

			void flip(bool _flipX, bool _flipY);

			void mirror(bool _mirrorX, bool _mirrorY);
			void mirrorX();
			void mirrorY();

			void offset(float _offsetX, float _offsetY);

			const sf::Transform& getTransformAdded() const;
			const sf::Transform& getInverseTransformAdded() const;

			void applyShit(sf::RenderStates& states) const;

	};

	class RectangleShape : public sf::RectangleShape, public TransformAddle
	{
		public:
			RectangleShape(const sf::Vector2f& size = sf::Vector2f(0, 0)) : sf::RectangleShape(size), TransformAddle(this)
			{
				this->setSize(sf::Vector2f(200.0f, 200.0f));
				setPosition(180.0f, 200.0f);
				//setRotation(-25.0f);
				//setShear(1.0f, 0.0f);
				//setScale(2.0f, 1.0f);
				
			}
			
			sf::FloatRect getGlobalBounds() const
			{
				return getTransformAdded().transformRect(sf::RectangleShape::getGlobalBounds());
			}
			sf::FloatRect getShitBounds() const
			{
				return sf::RectangleShape::getGlobalBounds();
			}
		private:
			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
			{
				applyShit(states);
				
				// I have to do this wizardly cuz the draw methods are private :(
				target.draw((sf::RectangleShape)(*this), states);
			}

	};
}
