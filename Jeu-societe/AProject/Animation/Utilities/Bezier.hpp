#pragma once

#ifndef _INC_BEZIER_HPP
#define _INC_BEZIER_HPP

#include <SFML/System.hpp>
#include <vector>

class BezierCurve
{
	private:
		std::vector<sf::Vector2f> m_controls;

	public:
		BezierCurve(void)
		{
			m_controls.resize(2);
			m_controls[0] = sf::Vector2f(0.0f, 0.0f);
			m_controls[1] = sf::Vector2f(1.0f, 1.0f);
		}

		//void AddControlPoint(std::vector<sf::Vector2f> _points);
		void AddControlPoint(const sf::Vector2f& _point)
		{
			for (int i = 0; i < this->m_controls.size()-1; i++)
			{
				if ((_point.x >= this->m_controls[i].x) && (_point.x <= this->m_controls[i + 1].x))
				{
					this->m_controls.insert(this->m_controls.begin() + i+1, _point);
					return;
				}
			}
		}
		//void AddControlPoint(float _x, float _y);

		float GetCoefficient(float _x)
		{
			std::vector<sf::Vector2f> beta(this->m_controls);
			size_t n = beta.size();

			for (int j = 1; j < n; j++)
			{
				for (int k = 0; k < n - j; k++)
				{
					beta[k].x = beta[k].x * (1.0f - _x) + beta[k + 1].x * _x;
				}
			}
			return beta[0].x;
		}

		static inline sf::Vector2f Interpolate(const sf::Vector2f& _from, const sf::Vector2f& _to, float _coefficient)
		{
			return sf::Vector2f(_from.x + ((_to.x - _from.x) * _coefficient),
								_from.y + ((_to.y - _from.y) * _coefficient));
		}
};




#endif