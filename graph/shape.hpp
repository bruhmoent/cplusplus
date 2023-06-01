#include "directives.hpp"
#include "point.hpp"

#ifndef SHAPE_HPP
#define SHAPE_HPP

struct Point;

struct Shape
{

	sf::Vector2f m_centerPoint;
	sf::VertexArray m_vertices;
	std::vector<std::shared_ptr<Point>> m_points;
	sf::VertexArray m_lines{ sf::Lines };

	Shape(sf::Vector2f center) : m_centerPoint(center){}

	template<typename T, typename... Args>
	void constructShape(std::shared_ptr<T> arg, Args... args);

	template<typename T>
	void addToPoints(std::shared_ptr<T> arg);

	template<typename T>
	void constructShape(std::shared_ptr<T> arg);

	void drawSelf(sf::RenderWindow& window);
};

template<typename T>
void Shape::constructShape(std::shared_ptr<T> arg)
{
	if (!std::is_same<T, Point>::value)
	{
		std::cerr << "Argument is not equal to type std::shared_ptr<Point>." << std::endl;
		return;
	}

	arg->m_position.x += m_centerPoint.x;
	arg->m_position.y += m_centerPoint.y;
	arg->m_visiblePoint.setPosition(arg->m_position.x, arg->m_position.y);

	addToPoints(arg);
}

template<typename T, typename... Args>
void Shape::constructShape(std::shared_ptr<T> arg, Args... args)
{
	constructShape(arg);
	constructShape(args...);
}

template<typename T>
void Shape::addToPoints(std::shared_ptr<T> arg)
{
	m_points.push_back(arg);
}

#endif