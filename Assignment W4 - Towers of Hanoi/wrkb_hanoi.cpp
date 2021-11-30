// ============================================
//  Mattia Peiretti, November 2021
//  https://mattiapeiretti.com/
//  Assignment H4 - N of Queens
// ============================================

#include <iostream>
#include <string>
#include <vector>
#include <stack>

#define LOG(X) std::cout << X << std::endl;

void DEBUG_printPegs(std::vector<std::stack<int>> station)
{

    std::string a, b, c;
    while (!(station.at(0).empty() && station.at(1).empty() && station.at(2).empty()))
    {
        if (!station.at(0).empty())
        {
            a = std::to_string(station.at(0).top());
            station.at(0).pop();
        }
        else
            a = " ";

        if (!station.at(1).empty())
        {
            b = std::to_string(station.at(1).top());
            station.at(1).pop();
        }
        else
            b = " ";

        if (!station.at(2).empty())
        {
            c = std::to_string(station.at(2).top());
            station.at(2).pop();
        }
        else
            c = " ";

        std::cout << "  " << a << "  " << b << "  " << c << std::endl;
    }
    std::cout << "  |  |  |  " << std::endl;
    std::cout << "===========" << std::endl;
}

//------------------------

bool isValidOperation(std::vector<std::stack<int>> &station, int diskSize, int peg)
{

    if (station.at(peg).empty())
        return 1;
    if (station.at(peg).top() > diskSize)
        return 1;

    return 0;
}

bool moveBetween(std::vector<std::stack<int>> &station, int peg1, int peg2)
{

    std::vector<char> pegsNames{'A', 'B', 'C'};
    DEBUG_printPegs(station);
    std::cin.get();
    // moving from 1 to 2
    if (!station.at(peg2).empty() && isValidOperation(station, station.at(peg2).top(), peg1))
    {
        std::cout << "Move disk " << station.at(peg1).top() << " from " << pegsNames.at(peg1) << " to " << pegsNames.at(peg2) << "." << std::endl;
        station.at(peg1).push(station.at(peg2).top());
        station.at(peg2).pop();

        return 1;
    }
    else if (!station.at(peg1).empty() && isValidOperation(station, station.at(peg1).top(), peg2))
    {
        std::cout << "Move disk " << station.at(peg1).top() << " from " << pegsNames.at(peg1) << " to " << pegsNames.at(peg2) << "." << std::endl;
        station.at(peg2).push(station.at(peg1).top());
        station.at(peg1).pop();

        return 1;
    }
    // moving from 2 to 1
    return 0;
}

bool solveHanoi(int NOfDisks, std::vector<std::stack<int>> &station, std::stack<int> endStack)
{
    if (station.at(2) == endStack)
        return 1;

    if (NOfDisks % 2 == 0)
    {
        if (moveBetween(station, 0, 1))
            if (moveBetween(station, 0, 2))
                if (moveBetween(station, 1, 2))
                    if (solveHanoi(NOfDisks, station, endStack))
                        return 1;
    }
    else
    {
        if (moveBetween(station, 0, 2))
            if (moveBetween(station, 0, 1))
                if (moveBetween(station, 1, 2))
                    if (solveHanoi(NOfDisks, station, endStack))
                        return 1;
    }
    return 0;
}

int main()
{
    static const int NUMBER_OF_PEGS = 3;
    static const int EMPTY_DISK_PLACE = 0;

    int numberOfDisks;

    std::cout
        << "How many disks should the tower consist of? ";
    std::cin >> numberOfDisks;

    if (std::cin.fail() || numberOfDisks < 1)
    {
        std::cout << "invalid input." << std::endl;
        return 1;
    }

    std::vector<std::stack<int>> station(NUMBER_OF_PEGS);

    for (int i = numberOfDisks; i >= 1; i--)
    {
        station.at(0).push(i);
    }

    //station.at(0).pop();
    //station.at(1).push(1);

    solveHanoi(numberOfDisks, station, station.at(0));

    DEBUG_printPegs(station);
    //LOG(isValidOperation(station, 1, 1));
    return 0;
}