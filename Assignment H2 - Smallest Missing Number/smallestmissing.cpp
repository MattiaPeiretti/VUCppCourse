// ============================================
//  Mattia Peiretti, November 2021
//  https://mattiapeiretti.com/
//  Assignment H2 - Smallest Missing Number
// ============================================

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

// Handy for debugging
#define LOG(x) std::cout << x << std::endl;
#define LOGV(x)             \
    for (auto &element : x) \
        std::cout << element << std::endl;

int findSmallestMissingNumber(std::vector<int> numbers)
{
    int offset = 0, currentNumber;

    // Sort vectors in crescent order
    std::sort(numbers.begin(), numbers.end());

    // If the first number +1 not present,
    // try the next number in the vector
    while (true)
    {
        currentNumber = numbers.at(offset);

        if (std::find(numbers.begin(), numbers.end(), currentNumber + 1) == numbers.end())
        {
            return currentNumber + 1;
        }
        else
        {
            offset++;
        }
    }
}

int main()
{
    std::string userInputString;
    int number, vectorSize;

    std::cout << "How many numbers? ";
    std::cin >> vectorSize;

    std::vector<int> listOfNumbers(vectorSize);

    std::cout << "Please enter the numbers ";

    // Clearing the cin, because we used the >>
    // operator above, but now we need to use getline()
    std::cin.ignore();
    std::getline(std::cin, userInputString);

    std::stringstream ss;
    ss.str(userInputString);

    for (int i = 0; i < vectorSize; i++)
    {
        ss >> number;
        listOfNumbers.at(i) = number;
    }

    std::cout << "The smallest missing number is "
              << findSmallestMissingNumber(listOfNumbers)
              << std::endl;

    return 0;
}
