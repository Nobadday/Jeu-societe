#include "GraphicAddons.hpp"


namespace sfMod
{

TransformAddle::TransformAddle(sf::Transformable* _test) :
m_shear							(0, 0),
m_flipX							(false),
m_flipY							(false),
m_mirrorX						(false),
m_mirrorY						(false),
m_offset						(0, 0),
m_transform						(),
m_transformNeedUpdate			(true),
m_inverseTransform				(),
m_inverseTransformNeedUpdate	(true)
{
	m_test = _test;
}

TransformAddle::~TransformAddle()
{
}

void TransformAddle::setShear(float _kX, float _kY)
{
	m_shear.x = _kX;
	m_shear.y = _kY;
	m_transformNeedUpdate = true;
	m_inverseTransformNeedUpdate = true;
}

void TransformAddle::setFlip(bool _flipX, bool _flipY)
{
	m_flipX = _flipX;
	m_flipY = _flipY;
	m_transformNeedUpdate = true;
	m_inverseTransformNeedUpdate = true;
}

void TransformAddle::setMirror(bool _mirrorX, bool _mirrorY)
{
	m_mirrorX = _mirrorX;
	m_mirrorY = _mirrorY;
	m_transformNeedUpdate = true;
	m_inverseTransformNeedUpdate = true;
}

void TransformAddle::setMirrorX(bool _condition)
{
	m_mirrorX = _condition;
	m_transformNeedUpdate = true;
	m_inverseTransformNeedUpdate = true;
}

void TransformAddle::setMirrorY(bool _condition)
{
	m_mirrorY = _condition;
	m_transformNeedUpdate = true;
	m_inverseTransformNeedUpdate = true;
}

void TransformAddle::setOffset(float _offsetX, float _offsetY)
{
	m_offset.x = _offsetX;
	m_offset.y = _offsetY;
	m_transformNeedUpdate = true;
	m_inverseTransformNeedUpdate = true;
}

void TransformAddle::shear(float _kX, float _kY)
{
	setShear(m_shear.x + _kX, m_shear.y + _kY);
}

void TransformAddle::flip(bool _flipX, bool _flipY)
{
	setFlip(m_flipX ^ _flipX, m_flipY ^ _flipY);
}

void TransformAddle::mirror(bool _mirrorX, bool _mirrorY)
{
	setMirror(m_mirrorX ^ _mirrorX, m_mirrorY ^ _mirrorY);
}

void TransformAddle::mirrorX()
{
	setMirrorX(!m_mirrorX);
}

void TransformAddle::mirrorY()
{
	setMirrorY(!m_mirrorY);
}

void TransformAddle::offset(float _offsetX, float _offsetY)
{
	setOffset(m_offset.x + _offsetX, m_offset.y + _offsetY);
}

const sf::Transform& TransformAddle::getTransformAdded() const
{
	// Recompute the combined transform if needed
	if (m_transformNeedUpdate)
	{
		// Math stuff here
		//m_transform = sf::Transform(1.0f, 1.0f, 0.0f, 
		//							0.0f, 1.0f, 0.0f, 
		//							0.0f, 0.0f, 1.0f);
		// Shearing factor

		//float sxc = m_shear.x;
		//float syc = m_shear.y;
		//float sxs = 1.0f;
		//float sys = 1.0f;
		//float tx = m_offset.x;
		//float ty = m_offset.y;
		
		sf::Vector2f position = m_test->getPosition();
		float sxc = 1.0f + (m_shear.y * m_shear.x);
		float syc = 1.0f;
		float sxs = m_shear.y;
		float sys = m_shear.x;
		float tx = m_offset.x - (position.x * m_shear.x);
		float ty = m_offset.y - (position.y * m_shear.y);
		
		/*if (m_mirrorX)
		{
			sxc *= -1.0f;
		}
		if (m_mirrorY)
		{
			syc *= -1.0f;
		}*/

		//if (m_flipX)
		//{
		//	sxc *= -1.0f;
		//}
		//if (m_flipY)
		//{
		//	syc *= -1.0f;
		//}
		printf("I know ur stuff (%f, %f)\n", position.x, position.y);

		m_transform = sf::Transform(sxc, sys, tx,
									sxs, syc, ty,
									0.f, 0.f, 1.f);
		
		//if (m_mirrorX)
		//{
		//	m_transform.scale(-1.0f, 1.0f);
		//}
		//if (m_mirrorY)
		//{
		//	m_transform.scale(1.0f, -1.0f);
		//}
		m_transformNeedUpdate = false;
	}
	
	return m_transform;
}

const sf::Transform& TransformAddle::getInverseTransformAdded() const
{
	// Recompute the inverse transform if needed
	if (m_inverseTransformNeedUpdate)
	{
		m_inverseTransform = getTransformAdded().getInverse();
		m_inverseTransformNeedUpdate = false;
	}

	return m_inverseTransform;
}

void TransformAddle::applyShit(sf::RenderStates& states) const
{
	states.transform *= getTransformAdded();
}

}
