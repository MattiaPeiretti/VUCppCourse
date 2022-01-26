#include <stdexcept>

template <typename T> class Vector{
    public:
        Vector(const int newSize); // throws std::out_of_range("Vector")
        ~Vector();
        int size() const { return actualSize; };
        void resize(const int newSize); // throws std::out_of_range("Vector")
        T& at(const int index); // throws std::out_of_range("Vector")
    private:
        T* data;
        int actualSize;
        int capacity;
};

//
// implement the missing interface functions here!
//