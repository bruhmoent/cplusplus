#pragma once

#include <iostream>
#include <vector>

#ifndef MAP_HPP
#define MAP_HPP

struct
Map
{
	char m_playingField[9][9]; //0-9
	std::vector< std::pair <int, int> > m_createdShips;
	int m_size = sizeof(m_playingField) / sizeof(m_playingField[0]);

	Map(char point)
	{
		for (int i = 0; i < m_size; i++)
		{
			for (int j = 0; j < m_size; j++)
			{
				m_playingField[i][j] = point;
			}
		}
	}

	void
	displayMap()
	{
		int rows = sizeof(m_playingField) / sizeof(m_playingField[0]);
		int columns = sizeof(m_playingField[0]) / sizeof(m_playingField[0][0]);

		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columns; j++)
			{
				std::cout << m_playingField[i][j];
			}
			std::cout << "\n";
		}
	}

	void
	addPoint(int coordinateX, int coordinateY, char point)
	{
		if (coordinateX >= m_size || coordinateY >= m_size)
		{
			std::cerr << "Coordinates (" << coordinateX << ", " << coordinateY << ") are out of reach.\n";
			return;
		}

		std::pair<int, int> coordinates = std::make_pair(coordinateY, coordinateX);
		if (std::find(m_createdShips.begin(), m_createdShips.end(), coordinates) != m_createdShips.end())
		{
			std::cerr << "Coordinates (" << coordinateX << ", " << coordinateY << ") already exist.\n";
			return;
		}

		m_playingField[coordinateY][coordinateX] = point;
		m_createdShips.push_back(coordinates);
		displayMap();
	}

};

#endif
