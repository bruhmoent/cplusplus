#include "ksiegarnia.hpp"

int main()
{
    std::unique_ptr<Ksiegarnia> ksieg(new Ksiegarnia());

    while (true)
    {
        std::cout << "Menu:\n";
        std::cout << "1. Dodaj ksiazke\n";
        std::cout << "2. Wyswietl wszystkie ksiazki\n";
        std::cout << "3. Wyjdz\n";

        int choice;
        std::cout << "Wybierz opcje: ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
        {
            std::string tytul = "", autor = "";
            int strony = 0, rok_wyd = 0;
            Gatunek gatunek;

            std::cout << "Podaj tytul: ";
            std::cin.ignore();
            std::getline(std::cin, tytul);

            std::cout << "Podaj autora: ";
            std::getline(std::cin, autor);

            std::cout << "Podaj liczbe stron: ";
            std::cin >> strony;

            std::cout << "Podaj rok wydania: ";
            std::cin >> rok_wyd;

            int gatunek_int = 0;
            std::cout << "Podaj gatunek (0 - Fantasy, 1 - Thriller, 2 - Naukowa, 3 - Romans): ";
            std::cin >> gatunek_int;

            gatunek = static_cast<Gatunek>(gatunek_int);

            std::unique_ptr<Ksiazka> newBook(new Ksiazka(tytul, autor, strony, rok_wyd, gatunek));
            ksieg->add_books(std::move(newBook));

            break;
        }
        case 2:
        {
            ksieg->print_all();
            break;
        }
        case 3:
        {
            return 0;
        }
        default:
        {
            std::cout << "Nieprawidlowy wybor. Sprobuj ponownie.\n";
            break;
        }
        }
    }

    return 0;
}