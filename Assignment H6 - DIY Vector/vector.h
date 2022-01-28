#include <stdexcept>

template <typename T>
class Vector
{
public:
    Vector(const int newSize); // throws std::out_of_range("Vector")
    ~Vector();
    int size() const { return actualSize; };
    void resize(const int newSize); // throws std::out_of_range("Vector")
    T &at(const int index);         // throws std::out_of_range("Vector")

private:
    T *data;
    int actualSize;
    int capacity;
};

template <typename T>
Vector<T>::Vector(const int newSize)
{
    if (newSize < 0)
        throw std::out_of_range("Vector");
    data = new T[newSize];
    actualSize = capacity = newSize;
}

template <typename T>
Vector<T>::~Vector()
{
    delete[] data;
}

template <typename T>
void Vector<T>::resize(const int newSize)
{
    if (newSize < 0)
        throw std::out_of_range("Vector");

    if (newSize > capacity)
    {
        // Backup old data
        T oldData[capacity];
        std::copy(data, data + actualSize, oldData);
        delete[] data;

        // Allocating new array
        data = new T[newSize];
        std::copy(oldData, oldData + actualSize, data);
        capacity = actualSize = newSize;
    }
    else
        actualSize = newSize;
}

template <typename T>
T &Vector<T>::at(const int index)
{
    if (index < 0 || (index + 1) > actualSize)
        throw std::out_of_range("Vector");
    else
        return data[index];
}
