// ============================================
//  Mattia Peiretti, January 2022
//  https://mattiapeiretti.com/
//  Assignment H5 - The Maze Generator
// ============================================

#define LOG(X) std::cout << X << std::endl;
#define LOGV(x)                       \
    std::cout << "( ";                \
    for (auto &element : x)           \
        std::cout << element << ", "; \
    std::cout << ")" << std::endl;

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>
#include <cctype>
#include <random>

using u32 = uint_least32_t;
using engine = std::mt19937;

enum directions
{
    none = -1,
    top = 0,
    right,
    bottom,
    left
};

// ========================

struct Coordinates
{
    int x = 0;
    int y = 0;
    directions direction;
    Coordinates(){};
    Coordinates(int _x, int _y, directions _direction = none)
    {
        x = _x;
        y = _y;
        direction = _direction;
    }
};

class Cell
{
private:
    std::vector<directions> connections;
    bool _isVisited = false;

public:
    Cell(){};
    std::vector<directions> getConnections();
    bool setConnection(directions direction);
    bool isConnectedTo(directions direction);
    bool setVisited();
    bool isVisited() { return false; }
    void DEBUGPrintCellData()
    {
        LOGV(connections)
        std::cout
            << std::endl;
    }
};

bool Cell::setConnection(directions direction)
{
    if (isConnectedTo(direction))
        return false;

    connections.push_back(direction);
    return true;
}

bool Cell::isConnectedTo(directions direction)
{
    return (std::find(connections.begin(), connections.end(), direction) != connections.end());
}

bool Cell::setVisited()
{
    _isVisited = true;
    return true;
}

// ========================

class Maze
{
private:
    std::vector<std::vector<Cell>> cells;
    int sizeX;
    int sizeY;

    void printHorizontalLine(int length, int row, bool endline);
    Cell &getCellByCoordinates(int x, int y);
    //-- Generation
    int visitedCount;
    std::stack<Coordinates> positionStack;
    std::uniform_int_distribution<u32> distribute2;
    std::uniform_int_distribution<u32> distribute3;
    u32 seed;
    engine generator;

public:
    Maze(int _sizeX, int _sizeY, int _seed);
    bool setCellConnection(int x, int y, directions direction);
    bool generateMaze(Coordinates currentCoords);
    const void printMaze();
    void DEBUG()
    {
        int x, y;
        x = y = 0;
        for (auto &row : cells)
        {
            for (Cell &element : row)
            {
                std::cout << x << "," << y << std::endl;
                element.DEBUGPrintCellData();
                y++;
            }
            y = 0;
            x++;
        }
    };
};

bool Maze::setCellConnection(int x, int y, directions direction)
{
    if (getCellByCoordinates(x, y).setConnection(direction) == false)
        return false;

    if (direction == top && y > 0)
        getCellByCoordinates(x, y - 1).setConnection(bottom);
    else if (direction == bottom && y < (sizeY - 1))
        getCellByCoordinates(x, y + 1).setConnection(top);
    else if (direction == left && x > 0)
        getCellByCoordinates(x - 1, y).setConnection(right);
    else if (direction == right && x < (sizeX - 1))
        getCellByCoordinates(x + 1, y).setConnection(left);
    else
        return false;
    return true;
}

// Maze::Maze(int _sizeX, int _sizeY) : cells(_sizeX, std::vector<Cell>(_sizeY))
Maze::Maze(int _sizeX, int _sizeY, int _seed)
{
    cells = std::vector<std::vector<Cell>>(_sizeX, std::vector<Cell>(_sizeY));

    sizeX = _sizeX;
    sizeY = _sizeY;
    // seed = _seed;

    // generator = engine(seed);
    // distribute2 = std::uniform_int_distribution<u32>(0, 1);
    // distribute3 = std::uniform_int_distribution<u32>(0, 2);
}

bool Maze::generateMaze(Coordinates currentCoords)
{
    // Setting up random engine with the seed

    // current cell visited
    positionStack.push(currentCoords);
    getCellByCoordinates(currentCoords.x, currentCoords.y).setVisited();

    visitedCount++;
    if (visitedCount == (sizeX * sizeY))
        return true;

    // check unvisited neighbors
    int availableNeighborsCount = 0;
    std::vector<Coordinates> availableNeighbors;
    // -top
    if ((currentCoords.y - 1) > 0 && !(getCellByCoordinates(currentCoords.x, currentCoords.y - 1).isVisited()))
    {
        availableNeighborsCount++;
        availableNeighbors.push_back(Coordinates(currentCoords.x, currentCoords.y - 1, top));
    }
    // -Right
    if ((currentCoords.x + 1) < (sizeX - 1) && !(getCellByCoordinates(currentCoords.x + 1, currentCoords.y).isVisited()))
    {
        availableNeighborsCount++;
        availableNeighbors.push_back(Coordinates(currentCoords.x + 1, currentCoords.y, right));
    }
    // -bottom
    if ((currentCoords.y + 1) < (sizeY - 1) && !(getCellByCoordinates(currentCoords.x, currentCoords.y + 1).isVisited()))
    {
        availableNeighborsCount++;
        availableNeighbors.push_back(Coordinates(currentCoords.x, currentCoords.y + 1, bottom));
    }
    // -left
    if ((currentCoords.x - 1) > 0 && !(getCellByCoordinates(currentCoords.x - 1, currentCoords.y).isVisited()))
    {
        availableNeighborsCount++;
        availableNeighbors.push_back(Coordinates(currentCoords.x - 1, currentCoords.y, left));
    }
    // set cell connection
    Coordinates nextCellCoords;
    if (availableNeighborsCount == 1)
        nextCellCoords = availableNeighbors.at(0);
    else if (availableNeighborsCount == 2)
        nextCellCoords = availableNeighbors.at(distribute2(generator));
    else if (availableNeighborsCount == 3)
        nextCellCoords = availableNeighbors.at(distribute2(generator));
    else
    {
        nextCellCoords = positionStack.top();
        positionStack.pop();
        generateMaze(nextCellCoords);
    }

    getCellByCoordinates(currentCoords.x, currentCoords.y).setConnection(nextCellCoords.direction);
    generateMaze(nextCellCoords);
    // will never reach
    return true;
}

void Maze::printHorizontalLine(int length, int row, bool endline = false)
{
    const static std::string junction = "+";
    const static std::string horizontalWall = "---";
    const static std::string OpenHorizontalWall = "   ";
    Cell currentCell;

    for (int x = 0; x < sizeX; x++)
    {
        if (row > 0)
            currentCell = getCellByCoordinates(x, row);

        std::cout << junction;
        if (endline)
            std::cout << horizontalWall;
        else if (currentCell.isConnectedTo(top) && row > 0)
            std::cout << OpenHorizontalWall;
        else
            std::cout << horizontalWall;

        if (x == (length - 1))
            std::cout << junction;
    }
}

const void Maze::printMaze()
{
    const static std::string verticalWall = "|";
    const static std::string openVerticalWall = " ";
    const static std::string filledCell = " . ";
    const static std::string emptyCell = "   ";
    std::string cellContent;
    Cell currentCell;

    for (int y = 0; y < sizeY; y++)
    {

        // will print slightly different line on final X loop
        printHorizontalLine(sizeX, y);
        std::cout << std::endl;

        for (int x = 0; x < sizeX; x++)
        {
            currentCell = getCellByCoordinates(x, y);
            cellContent = currentCell.isVisited() ? filledCell : emptyCell;
            if (currentCell.isConnectedTo(left) && x > 0)
                std::cout << openVerticalWall;
            else
                std::cout << verticalWall;
            std::cout << cellContent;

            if (x == (sizeX - 1))
                std::cout << verticalWall;
        }
        std::cout << std::endl;

        if (y == (sizeY - 1))
            // will print slightly different line on final X loop
            printHorizontalLine(sizeX, y, true);
    }
    std::cout << std::endl;
}

Cell &Maze::getCellByCoordinates(int x, int y)
{
    return cells.at(x).at(y);
}

// ========================

int main(int argc, char *argv[])
{
    // if (argc < 2)
    //     return -1;

    std::random_device os_seed;
    const u32 seed = os_seed();

    static const Coordinates startCoords(0, 0);

    Maze m(5, 5, seed);
    // std::cout << m.setCellConnection(0, 0, bottom);
    // std::cout << m.setCellConnection(4, 0, right);
    // std::cout << m.setCellConnection(1, 4, top);
    m.generateMaze(startCoords);
    std::cout
        << std::endl;
    m.DEBUG();
    m.printMaze();
}
