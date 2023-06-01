#include "shape.hpp"
#include "point.hpp"
#include "directives.hpp"

void Shape::drawSelf(sf::RenderWindow& window)
{
    if (m_points.empty())
    {
        throw std::runtime_error("Lacking data to draw.");
    }

    for (std::shared_ptr<Point>& p : m_points)
    {
        if (p)
        {
            window.draw(p->m_visiblePoint);
        }
    }
    
    for (int i = 1; i < m_points.size(); i++)
    {
        m_lines.append(sf::Vertex(sf::Vector2f(m_points[i]->m_position.x, m_points[i]->m_position.y), sf::Color::Blue));
        m_lines.append(sf::Vertex(sf::Vector2f(m_points[i - 1]->m_position.x, m_points[i - 1]->m_position.y), sf::Color::Blue));
    }

    //m_lines.append(sf::Vertex(sf::Vector2f(m_points[0]->m_position.x, m_points[0]->m_position.y), sf::Color::Blue));
    //m_lines.append(sf::Vertex(sf::Vector2f(m_points[m_points.size() - 1]->m_position.x, m_points[m_points.size() - 1]->m_position.y), sf::Color::Blue));
    window.draw(m_lines);
}