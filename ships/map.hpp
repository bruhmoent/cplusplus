#pragma once

#include "elements.hpp"
#include "element.hpp"

#ifndef MAP_HPP
#define MAP_HPP

struct 
Map
{
    char m_playingField[9][9]; //0-9
    int m_size;
    Elements& m_elements;

    Map(char point, Elements& elements) : m_size(9), m_elements(elements)
    {
        for (int i = 0; i < m_size; i++)
        {
            for (int j = 0; j < m_size; j++)
            {
                m_playingField[j][i] = point;
            }
        }
    }

    void 
    displayMap()
    {
        if (m_size <= 0)
        {
            std::cerr << "Invalid map size.\n";
            return;
        }

        for (int i = 0; i < m_size; i++)
        {
            for (int j = 0; j < m_size; j++)
            {
                m_playingField[j][i] = '.';
            }
        }

        for (const auto& element : m_elements.m_elements)
        {
            for (const std::pair<int, int>& coordinate : element.elementCoordinates)
            {
                int x = coordinate.first;
                int y = coordinate.second;

                if (x >= 0 && x < m_size && y >= 0 && y < m_size)
                {
                    m_playingField[y][x] = '*';
                }
                else
                {
                    std::cerr << "Invalid element coordinate: (" << x << ", " << y << ")\n";
                }
            }
        }

        for (int i = 0; i < m_size; i++)
        {
            for (int j = 0; j < m_size; j++)
            {
                std::cout << m_playingField[j][i];
            }
            std::cout << "\n";
        }
    }
};

#endif
