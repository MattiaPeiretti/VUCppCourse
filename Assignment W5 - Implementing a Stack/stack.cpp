// ============================================
//  Mattia Peiretti, January, 2022
//  https://mattiapeiretti.com/
//  Assignment W5 - Implementing a stack
// ============================================

#include <iostream>
#include <string>
#include <limits>
#include <vector>

class Stack
{
public:
    Stack(){};
    bool isEmpty() const;
    int top() const;
    int pop();
    void push(int i);
    std::string toString() const;

private:
    std::vector<int> elements;
};

bool Stack::isEmpty() const
{
    return elements.size() == 0;
}

int Stack::top() const
{
    if (isEmpty())
        throw std::runtime_error("stack is empty");
    return elements.back();
}

int Stack::pop()
{
    if (isEmpty())
        throw std::runtime_error("stack is empty");
    int poppedElement = elements.back();
    elements.pop_back();
    return poppedElement;
}

void Stack::push(int i)
{
    elements.push_back(i);
}

std::string Stack::toString() const
{
    const std::string beginning = "[";
    const std::string end = "]";
    const std::string separator = ",";

    std::string elementsToString = beginning;

    for (int i = elements.size() - 1; i >= 0; i--)
    {
        // do not add separator if first element in the list
        if (i != elements.size() - 1)
            elementsToString += separator;
        elementsToString += std::to_string(elements.at(i));
    }
    return elementsToString + end;
}

int main()
{
    Stack stack;

    while (true)
    {
        try
        {
            std::cout << "stack> ";

            std::string command;
            std::cin >> command;

            if ((command.compare("end") == 0) || std::cin.eof())
            {
                break;
            }
            else if (command.compare("top") == 0)
            {
                std::cout << stack.top() << std::endl;
            }
            else if (command.compare("pop") == 0)
            {
                std::cout << stack.pop() << std::endl;
            }
            else if (command == "push")
            {
                if (std::cin.eof())
                    break;
                int i;
                std::cin >> i;
                bool failed = std::cin.fail();
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (failed)
                {
                    throw std::runtime_error("not a number");
                }
                stack.push(i);
            }
            else if (command.compare("list") == 0)
            {
                std::cout << stack.toString() << std::endl;
                ;
            }
            else
            {
                throw std::runtime_error("invalid command");
            }
        }
        catch (std::runtime_error &e)
        {
            std::cout << "error: " << e.what() << std::endl;
        }
    }

    return 0;
}