#include "map.hpp"

Map g_map('-');

int main()
{
	char l_choice;
	int c_X, c_Y;

	do 
	{
		std::cout << "Select coordinate X: \n";
		std::cin >> c_X;

		std::cout << "Select coordinate Y: \n";
		std::cin >> c_Y;

		std::cout << std::endl;

		g_map.addPoint(c_X, c_Y, 'V');

		std::cout << "\nRepeat operation? (Y/N) \n";
		std::cin >> l_choice;

	} while (l_choice == 'Y');

	return 0;
}
