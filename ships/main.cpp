#include "elements.hpp"
#include "map.hpp"
#include "turns.hpp"

Elements g_elements;
Map g_map('.', g_elements);
Turns g_buildTurns(2);

std::vector<std::pair<int, int>>
shipCreation()
{
    int c_X, c_Y;

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

    return tmpCoordinates;
}

void 
shipDestruction(Elements& elements)
{
    int c_X, c_Y;

    std::cout << "Select coordinate X: ";
    std::cin >> c_X;

    std::cout << "Select coordinate Y: ";
    std::cin >> c_Y;

    std::cout << std::endl;

    for (Element& element : elements.m_elements)
    {
        element.setStateOfElement({ c_Y, c_X });
    }

}

int 
main()
{
    char l_choice;
    g_buildTurns.addFunction([]() { std::vector<std::pair<int, int>> tmp = shipCreation(); g_elements.addElement(Element(tmp, 100)); g_map.displayMap(); });
    g_buildTurns.take_turns(0);

    std::cout << "\n--Battle--\n";

    g_buildTurns.addFunction([]() { shipDestruction(g_elements); g_map.displayMap(); });
    g_buildTurns.take_turns(1);

    return 0;
}
