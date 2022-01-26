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
#include <tuple>
#include <stdlib.h>

// Random Number Generation engine setup
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

class Cell
{
private:
    std::vector<Directions> connections;
    bool _isVisited = false;

public:
    Cell(){};

    std::vector<Directions> getConnections() { return connections; };
    bool setConnection(Directions direction);
    bool isConnectedTo(Directions direction);

    void setVisited() { _isVisited = true; };
    bool isVisited() { return _isVisited; }
    void resetVisited() { _isVisited = false; };
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

// ========================

class Maze
{
private:
    int sizeX, sizeY;
    std::vector<Coordinates> availableNeighbors;
    std::vector<std::vector<Cell>> cells;

    // Generation
    std::stack<Coordinates> positionStack;
    std::uniform_int_distribution<u32> distribute2;
    std::uniform_int_distribution<u32> distribute3;
    engine generator;

    void printHorizontalLine(int length, int row, bool endline);

    // Generation
    bool generateWalls(Coordinates currentCoords, int visitedCount);
    bool setCellConnection(int x, int y, Directions direction);

public:
    Maze(int _sizeX, int _sizeY, int _seed);

    Cell &getCellByCoordinates(int x, int y);
    Cell &getCellByCoordinates(Coordinates coord);
    void generateMaze();
    const void printMaze();
};

Maze::Maze(int _sizeX, int _sizeY, int _seed)
{
    cells = std::vector<std::vector<Cell>>(_sizeX, std::vector<Cell>(_sizeY));

    sizeX = _sizeX;
    sizeY = _sizeY;

    generator = engine(_seed);
    distribute2 = std::uniform_int_distribution<u32>(0, 1);
    distribute3 = std::uniform_int_distribution<u32>(0, 2);
}

bool Maze::setCellConnection(int x, int y, Directions direction)
{
    if (getCellByCoordinates(x, y).setConnection(direction) == false)
        return false;

    // Add opposite connection to adjacent neighbors
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

void Maze::generateMaze()
{
    static const Coordinates startCoords(0, 0);
    generateWalls(startCoords, 0);
    for (std::vector<Cell> &row : cells)
        for (Cell &element : row)
            element.resetVisited();
}

bool Maze::generateWalls(Coordinates currentCoords, int visitedCount)
{
    if (visitedCount >= ((sizeX * sizeY) - 1))
        return true;

    getCellByCoordinates(currentCoords).setVisited();

    int availableNeighborsCount = 0;
    availableNeighbors = {};

    // -top
    if ((currentCoords.y - 1) >= 0 && !(getCellByCoordinates(getCoordinatesFromDirection(currentCoords, top)).isVisited()))
    {
        availableNeighborsCount++;
        availableNeighbors.push_back(Coordinates(currentCoords.x, currentCoords.y - 1, top));
    }
    // -Right
    if ((currentCoords.x + 1) < (sizeX) && !(getCellByCoordinates(getCoordinatesFromDirection(currentCoords, right)).isVisited()))
    {
        availableNeighborsCount++;
        availableNeighbors.push_back(Coordinates(currentCoords.x + 1, currentCoords.y, right));
    }
    // -bottom
    if ((currentCoords.y + 1) < (sizeY) && !(getCellByCoordinates(getCoordinatesFromDirection(currentCoords, bottom)).isVisited()))
    {
        availableNeighborsCount++;
        availableNeighbors.push_back(Coordinates(currentCoords.x, currentCoords.y + 1, bottom));
    }
    // -left
    if ((currentCoords.x - 1) >= 0 && !(getCellByCoordinates(getCoordinatesFromDirection(currentCoords, left)).isVisited()))
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
        positionStack.pop();
        if (generateWalls(positionStack.top(), visitedCount))
            return true;
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

Cell &Maze::getCellByCoordinates(Coordinates coord)
{
    return cells.at(coord.x).at(coord.y);
}

// ========================

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

    sizeY = atoi(argv[1]);
    sizeX = atoi(argv[2]);

    Maze m(sizeX, sizeY, seed);

    m.generateMaze();

    if (findPath(m, Coordinates(0, 0), Coordinates(sizeX - 1, sizeY - 1)))
    {
        m.printMaze();
        return 0;
    }

    return -1;
}
