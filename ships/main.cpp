#include "elements.hpp"
#include "map.hpp"

Elements g_elements;
Map g_map('.', g_elements);

int main()
{
    char l_choice;
    int c_X, c_Y;

    while (true)
    {
        std::vector<std::pair<int, int>> tmpCoordinates;

        while (true)
        {
            std::cout << "Select coordinate X (or enter -1 to stop adding coordinates): ";
            std::cin >> c_X;

            if (c_X == -1)
                break;

            std::cout << "Select coordinate Y: ";
            std::cin >> c_Y;

            std::cout << std::endl;

            std::pair<int, int> coordinates = std::make_pair(c_Y, c_X);

            bool coordinatesExist = false;
            for (const auto& coordinate : tmpCoordinates)
            {
                if (coordinate == coordinates)
                {
                    coordinatesExist = true;
                    break;
                }
            }

            if (coordinatesExist)
            {
                std::cerr << "These coordinates are already used. Please enter different coordinates.\n";
                continue;
            }

            tmpCoordinates.push_back(coordinates);
        }

        g_elements.addElement(Element(tmpCoordinates, 100));

        for (const Element& element : g_elements.m_elements)
        {
            std::cout << "Element Coordinates: ";
            for (const auto& coordinate : element.elementCoordinates)
            {
                std::cout << "(" << coordinate.first << ", " << coordinate.second << ") ";
            }
            std::cout << std::endl;
        }

        g_map.displayMap();

        std::cout << "Do you want to continue playing? (Y/N): ";
        std::cin >> l_choice;

        if (l_choice != 'Y')
            break;
    }

    return 0;
}
