#include <iostream>

template<typename T>
class Vector {
public:
    ~Vector() {
        delete[] elements;
    }
    
    Vector(std::initializer_list<T> init_list) : capacity(init_list.size()), elements(new T[capacity]) {
        std::copy(init_list.begin(), init_list.end(), elements);
    }

    Vector(const Vector& other) : capacity(other.capacity), elements(new T[capacity]) {
        std::copy(other.begin(), other.end(), elements);
    }
    
    Vector& operator=(Vector other) {
        swap(*this, other);
        return *this;
    }

    T& operator[](const int index) {
        if (index >= capacity || index < 0) {
            throw std::out_of_range("Index out of bounds");
        } else {
            return elements[index];
        }
    }

    void remove_at(const int index) {
        if (index >= capacity || index < 0) {
            throw std::out_of_range("Index out of bounds");
        }

        T* temp_vec = new T[capacity - 1];
        for (int i = 0, iter = 0; i < capacity; ++i) {
            if (i != index) {
                temp_vec[iter++] = elements[i];
            }
        }

        delete[] elements;
        elements = temp_vec;
        --capacity;
    }

    void insert_at(const int index, const T& value) {
        if (index > capacity || index < 0) {
            throw std::out_of_range("Index out of bounds");
        }

        T* new_elements = new T[capacity + 1];
        for (int i = 0, j = 0; i <= capacity; ++i) {
            if (i == index) {
                new_elements[i] = value;
            } else {
                new_elements[i] = elements[j++];
            }
        }

        delete[] elements;
        elements = new_elements;
        ++capacity;
    }

    void push(const T& value) {
        resize_if_needed();
        elements[capacity++] = value;
    }

    template<typename... Args>
    void push(Args... args) {
        (push(args), ...);
    }

    inline int size() const {
        return capacity;
    }

    T* begin() {
        return elements;
    }

    T* end() {
        return elements + capacity;
    }

    const T* begin() const {
        return elements;
    }

    const T* end() const {
        return elements + capacity;
    }

private:
    int capacity = 0;
    T* elements = nullptr;

    void resize_if_needed() {
        if (capacity == 0) {
            elements = new T[1];
        } else if (capacity % 5 == 0) {
            T* new_elements = new T[capacity * 2];
            std::copy(elements, elements + capacity, new_elements);
            delete[] elements;
            elements = new_elements;
        }
    }
};
