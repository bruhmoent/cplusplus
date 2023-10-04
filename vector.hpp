#include <cstddef>
#include <algorithm>

template<typename T>
class Vector
{
public:
    Vector() noexcept
        : m_alloc(nullptr), m_size(0)
    {}

    Vector(std::initializer_list<T> args) noexcept
        : m_alloc(new T[args.size()]), m_size(args.size())
    {
        std::copy(args.begin(), args.end(), m_alloc);
    }

    [[nodiscard]] constexpr T* begin() noexcept
    {
        return m_alloc;
    }

    [[nodiscard]] constexpr const T* end() noexcept
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
        if (m_size == 0)
            return;

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

    [[nodiscard]] constexpr int size() const noexcept
    {
        return m_size;
    }

    ~Vector()
    {
        delete[] m_alloc;
    }

private:
    T* m_alloc;
    std::size_t m_size;
};
