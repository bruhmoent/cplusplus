#include <iostream>
#include <algorithm>

template<typename T>
class DynamicArray
{
public:
    DynamicArray()
    {
        m_alloc = nullptr;
        m_size = 0;
    }

    DynamicArray(std::initializer_list<T> args)
    {
        m_size = args.size();
        m_alloc = new T[m_size];
        std::copy(args.begin(), args.end(), m_alloc);
    }

    T* begin()
    {
        return m_alloc;
    }

    T* end()
    {
        return m_alloc + m_size;
    }

    void push_back(T arg)
    {
        T* new_alloc = new T[m_size + 1];

        for (std::size_t i = 0; i < m_size; ++i) {
            new_alloc[i] = m_alloc[i];
        }

        new_alloc[m_size] = arg;

        delete[] m_alloc;
        m_alloc = new_alloc;

        ++m_size;
    }

    void remove(T value)
    {
        T* new_alloc = new T[m_size - 1];
        std::size_t new_index = 0;

        for (std::size_t i = 0; i < m_size; ++i) {
            if (m_alloc[i] != value) {
                new_alloc[new_index] = m_alloc[i];
                ++new_index;
            }
        }

        delete[] m_alloc;
        m_alloc = new_alloc;

        --m_size;
    }

    int size()
    {
        return m_size;
    }

    ~DynamicArray()
    {
        delete[] m_alloc;
    }

private:
    T* m_alloc;
    std::size_t m_size;
};
