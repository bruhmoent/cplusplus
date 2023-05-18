#include <iostream>
#include <vector>

#ifndef ELEMENT_HPP
#define ELEMENT_HPP

struct 
Element
{

    std::vector<std::pair<int, int>> m_elementCoordinates;
    std::vector<std::pair<int, int>> m_hitCoordinates;
    int m_health;

    Element(std::vector<std::pair<int, int>> coordinates, int hlth) : m_elementCoordinates(coordinates), m_health(hlth) {}

    void 
    setStateOfElement(const std::pair<int, int>& coordinates)
    {

        for (const auto& elementCoordinate : m_elementCoordinates)
        {
            if (elementCoordinate.first == coordinates.first && elementCoordinate.second == coordinates.second)
            {
                m_hitCoordinates.push_back(coordinates);
            }
        }

    }

};

#endif
