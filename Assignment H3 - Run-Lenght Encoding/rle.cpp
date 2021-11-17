// ============================================
//  Mattia Peiretti, November 2021
//  https://mattiapeiretti.com/
//  Assignment H3 - Run-length Encoding
// ============================================

#include <iostream>
#include <string>
#include <vector>

int main()
{
    std::string rawData;
    std::vector<char> characters;
    std::vector<int> charactersAmount;

    std::cout << "Enter the data to be compressed: ";
    std::getline(std::cin, rawData);

    if (rawData.find_first_not_of("abcdefghijklmnopqrstuvwxyz") != std::string::npos)
        return -1;

    return 0;
}