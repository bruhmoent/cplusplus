#include <string>
#include <iostream>

#ifndef KSIAZKA_HPP
#define KSIAZKA_HPP

enum Gatunek
{
	Fantasy = 0,
	Thriller = 1,
	Naukowa = 2,
	Romans = 3
};

class Ksiazka
{
	public:
		Ksiazka(std::string t, std::string a, int i, int r, Gatunek g) : m_tytul(t), m_autor(a), m_ilosc_stron(i), m_rok_wydania(r), m_gatunek(g)
		{
			m_enum_string = get_val(g);
		}

		std::string get_val(const int index)
		{
			switch (index)
			{
				case 0:
					return "Fantasy";
				case 1:
					return "Thriller";
				case 2:
					return "Naukowa";
				case 3:
					return "Romans";
			}
		}

		int wiek_ksiazki(const int rok_obecny)
		{
			return rok_obecny - m_rok_wydania;
		}
		
		inline void opis()
		{
			std::cout << m_tytul << " autorstwa: " << m_autor << ", " << m_ilosc_stron << " stron." << " Ksiazka ma " << wiek_ksiazki(2024) << " lat." << " Gatunek ksiazki to: " << m_enum_string << std::endl;
		}
    private:
		std::string m_tytul = "";
		std::string m_autor = "";
		std::string m_enum_string = "";
		int m_ilosc_stron = 0;
		int m_rok_wydania = 0;
		Gatunek m_gatunek;
};

#endif // KSIAZKA_HPP