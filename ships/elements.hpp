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
            for (const auto& existingCoordinate : existingElement.elementCoordinates)
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

        for (const auto& coordinate : e.elementCoordinates)
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
        int minX = e.elementCoordinates[0].first;
        int maxX = e.elementCoordinates[0].first;
        int minY = e.elementCoordinates[0].second;
        int maxY = e.elementCoordinates[0].second;

        for (const auto& coordinate : e.elementCoordinates)
        {
            minX = std::min(minX, coordinate.first);
            maxX = std::max(maxX, coordinate.first);
            minY = std::min(minY, coordinate.second);
            maxY = std::max(maxY, coordinate.second);
        }

        int width = maxX - minX + 1;
        int height = maxY - minY + 1;

        int expectedSize = e.elementCoordinates.size();
        return (width * height) == expectedSize;
    }

    void
    addElement(Element e)
    {
        for (const Element& existingElement : m_elements)
        {
            for (const auto& coordinate : existingElement.elementCoordinates)
            {
                if (std::find(e.elementCoordinates.begin(), e.elementCoordinates.end(), coordinate) != e.elementCoordinates.end())
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

        if (e.elementCoordinates.size() < 2)
        {
            std::cerr << "Invalid element. There must be a minimum of 2 coordinates.\n";
            return;
        }

        m_elements.push_back(e);
    }

    void 
        deleteElement(const Element& e)
    {
        for (auto it = m_elements.begin(); it != m_elements.end(); )
        {
            const Element& existingElement = *it;
            bool shouldErase = false;

            for (const auto& coordinate : existingElement.elementCoordinates)
            {
                if (std::find(e.elementCoordinates.begin(), e.elementCoordinates.end(), coordinate) != e.elementCoordinates.end())
                {
                    shouldErase = true;
                    break;
                }
            }

            if (shouldErase)
            {
                it = m_elements.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

};

#endif
