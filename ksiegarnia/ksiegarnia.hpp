#include "ksiazka.hpp"
#include <vector>
#include <memory>
#include <utility>

#ifndef KSIEGARNIA_HPP
#define KSIEGARNIA_HPP

class Ksiegarnia
{
public:
    template <typename... Args>
    void add_books(Args&&... books)
    {
        (add_books_helper(std::forward<Args>(books)), ...);
    }

    template <typename T>
    void add_books_helper(T&& book)
    {
        m_ksiazki.push_back(std::forward<T>(book));
    }

	void print_all()
	{
		for (auto& ks : m_ksiazki)
		{
			ks->opis();
		}
	}
private:
	std::vector<std::unique_ptr<Ksiazka>> m_ksiazki;
};

#endif // KSIEGARNIA_HPP