#pragma once

#include "element.hpp"

#ifndef ELEMENTS_HPP
#define ELEMENTS_HPP

struct
Elements
{

    std::vector<Element> m_elements;

    bool
    isCoordinateTaken(const std::pair<int, int>& coordinate) const
    {
        for (const Element& existingElement : m_elements)
        {
            for (const auto& existingCoordinate : existingElement.m_elementCoordinates)
            {
                if (existingCoordinate == coordinate)
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool
    checkForBorder(const Element& e) const
    {
        std::vector<std::pair<int, int>> directions = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

        for (const auto& coordinate : e.m_elementCoordinates)
        {
            int x = coordinate.first;
            int y = coordinate.second;

            for (const auto& direction : directions)
            {
                int neighborX = x + direction.first;
                int neighborY = y + direction.second;
                if (isCoordinateTaken({ neighborX, neighborY }))
                {
                    return false;
                }
            }
        }

        return true;
    }

    bool
    isValidArea(const Element& e) const
    {

        int minX = e.m_elementCoordinates[0].first;
        int maxX = e.m_elementCoordinates[0].first;
        int minY = e.m_elementCoordinates[0].second;
        int maxY = e.m_elementCoordinates[0].second;

        for (const auto& coordinate : e.m_elementCoordinates)
        {
            minX = std::min(minX, coordinate.first);
            maxX = std::max(maxX, coordinate.first);
            minY = std::min(minY, coordinate.second);
            maxY = std::max(maxY, coordinate.second);
        }

        int width = maxX - minX + 1;
        int height = maxY - minY + 1;

        int expectedSize = e.m_elementCoordinates.size();
        return (width * height) == expectedSize;

    }

    void
    addElement(Element e)
    {

        for (const Element& existingElement : m_elements)
        {
            for (const auto& coordinate : existingElement.m_elementCoordinates)
            {
                if (std::find(e.m_elementCoordinates.begin(), e.m_elementCoordinates.end(), coordinate) != e.m_elementCoordinates.end())
                {
                    std::cerr << "This element already exists.\n";
                    return;
                }
            }
        }

        if (!isValidArea(e))
        {
            std::cerr << "Invalid element placement. The area does not match the ship's size and shape.\n";
            return;
        }

        if (!checkForBorder(e))
        {
            std::cerr << "Invalid element placement. The element must have a one-cell border around it.\n";
            return;
        }

        if (e.m_elementCoordinates.size() < 2)
        {
            std::cerr << "Invalid element. There must be a minimum of 2 coordinates.\n";
            return;
        }

        m_elements.push_back(e);

    }

    void 
    deleteElement(const Element& e)
    {

        auto it = std::remove_if(m_elements.begin(), m_elements.end(), [&](const Element& existingElement) {
            for (const auto& coordinate : existingElement.m_elementCoordinates)
            {
                if (std::find(e.m_elementCoordinates.begin(), e.m_elementCoordinates.end(), coordinate) != e.m_elementCoordinates.end())
                {
                    return true;
                }
            }
            return false;
            });

        m_elements.erase(it, m_elements.end());

    }

};

#endif
