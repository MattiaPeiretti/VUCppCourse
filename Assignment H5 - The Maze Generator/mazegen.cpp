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
#include <stdlib.h>

using u32 = uint_least32_t;
using engine = std::mt19937;

enum Directions
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
    Directions direction;
    Coordinates(){};
    Coordinates(int _x, int _y, Directions _direction = none)
    {
        x = _x;
        y = _y;
        direction = _direction;
    }
    bool operator==(const Coordinates &rhs) const
    {
        return std::tie(x, y, direction) == std::tie(rhs.x, rhs.y, rhs.direction);
    }
};

class Cell
{
private:
    std::vector<Directions> connections;
    bool _isVisited = false;

public:
    Cell(){};
    std::vector<Directions> getConnections();
    bool setConnection(Directions direction);
    bool isConnectedTo(Directions direction);
    bool setVisited();
    bool resetVisited();
    bool isVisited() { return _isVisited; }
    void DEBUGPrintCellData()
    {
        LOGV(connections)
        std::cout
            << std::endl;
    }
};

bool Cell::setConnection(Directions direction)
{
    if (isConnectedTo(direction))
        return false;

    connections.push_back(direction);
    return true;
}

bool Cell::isConnectedTo(Directions direction)
{
    return (std::find(connections.begin(), connections.end(), direction) != connections.end());
}

bool Cell::setVisited()
{
    _isVisited = true;
    return true;
}

bool Cell::resetVisited()
{
    _isVisited = false;
    return true;
}
std::vector<Directions> Cell::getConnections()
{
    return connections;
}

// ========================

class Maze
{
private:
    std::vector<std::vector<Cell>> cells;
    int sizeX;
    int sizeY;
    std::vector<Coordinates> availableNeighbors;

    void printHorizontalLine(int length, int row, bool endline);
    //-- Generation

    std::stack<Coordinates> positionStack;
    std::uniform_int_distribution<u32> distribute2;
    std::uniform_int_distribution<u32> distribute3;
    u32 seed;
    engine generator;

public:
    Maze(int _sizeX, int _sizeY, int _seed);
    Cell &getCellByCoordinates(int x, int y);
    bool setCellConnection(int x, int y, Directions direction);
    void generateMaze();
    const void printMaze();
    bool generateWalls(Coordinates currentCoords, int visitedCount);

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

bool Maze::setCellConnection(int x, int y, Directions direction)
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

    generator = engine(_seed);
    distribute2 = std::uniform_int_distribution<u32>(0, 1);
    distribute3 = std::uniform_int_distribution<u32>(0, 2);
}

void Maze::generateMaze()
{
    static const Coordinates startCoords(0, 0);
    generateWalls(startCoords, 0);
    for (auto &row : cells)
        for (Cell &element : row)
            element.resetVisited();
}

bool Maze::generateWalls(Coordinates currentCoords, int visitedCount)
{
    // Setting up random engine with the seed
    //LOG((sizeX * sizeY) - 1);
    if (visitedCount >= ((sizeX * sizeY) - 1))
        return true;

    // current cell visited

    getCellByCoordinates(currentCoords.x, currentCoords.y).setVisited();

    // check unvisited neighbors
    int availableNeighborsCount = 0;
    availableNeighbors = {};
    // -top
    if ((currentCoords.y - 1) >= 0 && !(getCellByCoordinates(currentCoords.x, currentCoords.y - 1).isVisited()))
    {
        availableNeighborsCount++;
        //LOG("TOP")
        availableNeighbors.push_back(Coordinates(currentCoords.x, currentCoords.y - 1, top));
    }
    // -Right
    if ((currentCoords.x + 1) < (sizeX) && !(getCellByCoordinates(currentCoords.x + 1, currentCoords.y).isVisited()))
    {
        availableNeighborsCount++;
        //LOG("RIGHT")

        availableNeighbors.push_back(Coordinates(currentCoords.x + 1, currentCoords.y, right));
    }

    // -bottom
    if ((currentCoords.y + 1) < (sizeY) && !(getCellByCoordinates(currentCoords.x, currentCoords.y + 1).isVisited()))
    {
        availableNeighborsCount++;
        //LOG("BOTTOM")

        availableNeighbors.push_back(Coordinates(currentCoords.x, currentCoords.y + 1, bottom));
    }
    // -left
    if ((currentCoords.x - 1) >= 0 && !(getCellByCoordinates(currentCoords.x - 1, currentCoords.y).isVisited()))
    {
        availableNeighborsCount++;
        //LOG("LEFT")

        availableNeighbors.push_back(Coordinates(currentCoords.x - 1, currentCoords.y, left));
    }
    // set cell connection
    Coordinates nextCellCoords;
    if (availableNeighborsCount == 1)
        nextCellCoords = availableNeighbors.at(0);
    else if (availableNeighborsCount == 2)
        //LOG(distribute2(generator))
        nextCellCoords = availableNeighbors.at(distribute2(generator));
    else if (availableNeighborsCount == 3)
        nextCellCoords = availableNeighbors.at(distribute2(generator));
    else
    {
        positionStack.pop();
        if (generateWalls(positionStack.top(), visitedCount))
        {

            return true;
        }
    }

    visitedCount++;
    positionStack.push(currentCoords);

    setCellConnection(currentCoords.x, currentCoords.y, nextCellCoords.direction);
    generateWalls(nextCellCoords, visitedCount);
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
    const static std::string filledCell = " O ";
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
            cellContent = getCellByCoordinates(x, y).isVisited() ? filledCell : emptyCell;
            if (getCellByCoordinates(x, y).isConnectedTo(left) && x > 0)
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

Coordinates getCoordinatesFromDirection(Coordinates initialCoords, Directions direction)
{
    if (direction == top)
        initialCoords.y--;
    else if (direction == right)
        initialCoords.x++;
    else if (direction == bottom)
        initialCoords.y++;
    else if (direction == left)
        initialCoords.x--;
    else
        return initialCoords; // Return initial coords if already in None state
    return initialCoords;
}

bool findPath(Maze &m, Coordinates from, Coordinates to)
{
    std::vector<Directions> currentConnections;
    // 1. M.at(from).visited <- true
    m.getCellByCoordinates(from.x, from.y)
        .setVisited();
    // 2. if from equals to, return true
    if (from == to)
        return true;
    // 3. neighbours <- list of all direct neighbours of from that can be reached (that are not blocked by walls)
    Coordinates currentNeighbor;
    currentConnections = m.getCellByCoordinates(from.x, from.y).getConnections();
    // 4. for all n in neighbours:
    for (Directions &connection : currentConnections)
    {
        currentNeighbor = getCoordinatesFromDirection(from, connection);
        //      if M.at(n).visited == false
        if (m.getCellByCoordinates(currentNeighbor.x, currentNeighbor.y).isVisited() == false)
        {
            //          if findPath(n,to) == true, return true
            if (findPath(m, currentNeighbor, to) == true)
                return true;
        }
    }
    // 5. M.at(from).visited <- false
    m.getCellByCoordinates(from.x, from.y)
        .resetVisited();
    // 6. return false
    return false;
}
int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 4)
        return -1;

    u32 seed;
    int sizeX, sizeY;

    if (argc == 3)
    {
        std::random_device os_seed;
        seed = os_seed();
    }
    else
        seed = atoi(argv[3]);

    sizeX = atoi(argv[1]);
    sizeY = atoi(argv[2]);

    // if (Coordinates(0, 0) == Coordinates(1, 0))
    //     LOG("aaa");

    Maze m(sizeX, sizeY, seed);

    m.generateMaze();
    std::cout
        << std::endl;
    if (findPath(m, Coordinates(0, 0), Coordinates(sizeX - 1, sizeY - 1)))
        LOG("Yay!");

    m.printMaze();
}
