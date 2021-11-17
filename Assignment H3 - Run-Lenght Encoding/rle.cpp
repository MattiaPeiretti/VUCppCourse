// ============================================
//  Mattia Peiretti, November 2021
//  https://mattiapeiretti.com/
//  Assignment H3 - Run-length Encoding
// ============================================

#include <iostream>
#include <string>
#include <vector>
#include <regex>

#define LOG(X) std::cout << X << std::endl;
#define LOGV(x)                       \
    std::cout << "( ";                \
    for (auto &element : x)           \
        std::cout << element << ", "; \
    std::cout << ")" << std::endl;

int main()
{
    std::string rawData;
    std::vector<char> characters;
    std::vector<int> runLengths;

    std::cout << "Enter the data to be compressed: ";
    std::getline(std::cin, rawData);

    if (!std::regex_match(rawData, std::regex("^[a-z]+$")))
    {
        std::cout << "error: invalid input" << std::endl;
        return -1;
    }

    char currentCharacter;
    int characterPosition = -1;

    for (std::string::size_type i = 0; i < rawData.size(); i++)
    {
        currentCharacter = rawData[i];
        if (currentCharacter != rawData[i - 1])
        {
            characters.push_back(currentCharacter);
            runLengths.push_back(0);
            characterPosition++;
        }

        runLengths.at(characterPosition) = runLengths.at(characterPosition) + 1;
    }

    std::cout << "The compressed data is: ";

    for (int i = 0; i <= characters.size() - 1; i++)
    {
        std::cout << runLengths.at(i) << characters.at(i);
    }

    return 0;
}