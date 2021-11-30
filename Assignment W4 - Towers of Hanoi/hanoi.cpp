// ============================================
//  Mattia Peiretti, November 2021
//  https://mattiapeiretti.com/
//  Assignment W4 - Towers of Hanoi
// ============================================

#include <iostream>
#include <string>
#include <vector>
#include <stack>

//------------------------

enum
{
    pegA,
    pegB,
    pegC,
};

bool isValidOperation(std::vector<std::stack<int>> &station, int diskSize, int peg)
{
    if (station.at(peg).empty())
        return 1;
    if (station.at(peg).top() > diskSize)
        return 1;
    return 0;
}

// Moves a disk between two pegs in the first possible direction.
bool moveBetween(std::vector<std::stack<int>> &station, int peg1, int peg2)
{
    std::vector<char> pegsNames{'A', 'B', 'C'};

    // moving from 1 to 2
    if (!station.at(peg1).empty() && isValidOperation(station, station.at(peg1).top(), peg2))
    {
        std::cout << "Move disk "
                  << station.at(peg1).top()
                  << " from "
                  << pegsNames.at(peg1)
                  << " to "
                  << pegsNames.at(peg2)
                  << "."
                  << std::endl;

        station.at(peg2).push(station.at(peg1).top());
        station.at(peg1).pop();

        return 1;
    }
    // moving from 2 to 1
    else if (!station.at(peg2).empty() && isValidOperation(station, station.at(peg2).top(), peg1))
    {
        std::cout << "Move disk "
                  << station.at(peg2).top()
                  << " from "
                  << pegsNames.at(peg2)
                  << " to "
                  << pegsNames.at(peg1)
                  << "."
                  << std::endl;

        station.at(peg1).push(station.at(peg2).top());
        station.at(peg2).pop();

        return 1;
    }

    return 0;
}

bool solveHanoi(int NOfDisks, std::vector<std::stack<int>> &station, std::stack<int> endStack)
{

    if (station.at(pegC) == endStack)
        return 1;

    if (NOfDisks % 2 == 0)
    {
        if (moveBetween(station, pegA, pegB))
            if (moveBetween(station, pegA, pegC))
                if (moveBetween(station, pegB, pegC))
                    if (solveHanoi(NOfDisks, station, endStack))
                        return 1;
    }
    else
    {
        if (moveBetween(station, pegA, pegC))
            if (moveBetween(station, pegA, pegB))
                if (moveBetween(station, pegB, pegC))
                    if (solveHanoi(NOfDisks, station, endStack))
                        return 1;
    }
    return 0;
}

int main()
{
    static const int NUMBER_OF_PEGS = 3;

    std::vector<std::stack<int>> station(NUMBER_OF_PEGS);
    int numberOfDisks;

    std::cout << "How many disks should the tower consist of? ";
    std::cin >> numberOfDisks;

    if (std::cin.fail() || numberOfDisks < 1)
    {
        std::cout << "invalid input." << std::endl;
        return 1;
    }

    for (int i = numberOfDisks; i >= 1; i--)
    {
        station.at(pegA).push(i);
    }

    solveHanoi(numberOfDisks, station, station.at(pegA));
    return 0;
}