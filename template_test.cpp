#include <iostream>
#include <tuple>

template <typename... Args>
struct Test {
    template <std::size_t Index, typename T>
    static void print_element(const T& t) {
        std::cout << t << " at index " << Index << "\n";
    }

    template <std::size_t... Indices>
    static void print_sequence_impl(const std::tuple<Args...>& t, std::index_sequence<Indices...>) {
        ((print_element<Indices>(std::get<Indices>(t))), ...);
    }

    static void print_sequence(const Args&... args) {
        constexpr std::size_t size = sizeof...(Args);
        std::tuple<Args...> argTuple(args...);
        print_sequence_impl(argTuple, std::make_index_sequence<size>());
    }
};

template <typename... Ts>
struct PrintTuple {
    static void print(const std::tuple<Ts...>& t) {
        print_tuple_impl(t, std::make_index_sequence<sizeof...(Ts)>());
    }

private:
    template <std::size_t Index>
    static void print_tuple_impl(const std::tuple<Ts...>& t, std::index_sequence<Index>) {
        std::cout << std::get<Index>(t) << std::endl;
    }

    template <std::size_t Index, std::size_t... Rest>
    static void print_tuple_impl(const std::tuple<Ts...>& t, std::index_sequence<Index, Rest...>) {
        std::cout << std::get<Index>(t) << ", ";
        print_tuple_impl(t, std::index_sequence<Rest...>());
    }
};

int main() {
    std::tuple<int, float, std::string> myTuple(42, 3.14f, "Hello");

    PrintTuple<int, float, std::string>::print(myTuple);
    std::cout << "\n";
    
    Test<int, double, char> test;
    test.print_sequence(42, 3.14, 'a');
    return 0;
}
