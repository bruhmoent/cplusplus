template<typename T>
void showType(const T& t)
{
    int status;
    char* demangled = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
    if (status == 0)
    {
        std::cout << "Typ: " << demangled << std::endl;
        std::free(demangled);
    }
    else { std::cout << "Typ: " << typeid(T).name() << std::endl; }
    pudelko p; //parsing our struct
    //in order for this to work we need to declare a begin() and end() function in our struct. Those functions should return the beginning and the end variable of the struct.
    for (auto& value : p)
    {
    std::cout << value << std::endl;
    }
}
