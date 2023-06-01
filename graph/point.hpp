#include "directives.hpp"

#ifndef POINT_HPP
#define POINT_HPP

struct Point
{

	sf::Vector2f m_position;
	sf::CircleShape m_visiblePoint;

	Point(sf::Vector2f position) : m_position(position)
	{

		m_visiblePoint.setRadius(5);
		m_visiblePoint.setOutlineColor(sf::Color::Red);
		m_visiblePoint.setOutlineThickness(1);
		m_visiblePoint.setPosition(m_position.x, m_position.y);

	}

};

#endif