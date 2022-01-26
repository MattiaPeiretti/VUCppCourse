// ============================================
//  Mattia Peiretti, January 2022
//  https://mattiapeiretti.com/
//  Assignment H5 - The Maze Generator
// ============================================

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

struct Coordinates
{
    Coordinates(){};
    Coordinates(int _x, int _y, Directions _direction = none);

    int x = 0;
    int y = 0;
    Directions direction;

    // Overloading == operator to allow correct comparisons
    bool operator==(const Coordinates &rhs) const
    {
        return std::tie(x, y, direction) == std::tie(rhs.x, rhs.y, rhs.direction);
    }
};

Coordinates::Coordinates(int _x, int _y, Directions _direction)
{
    x = _x;
    y = _y;
    direction = _direction;
}

Coordinates CalculateCoordinatesFromDirection(Coordinates initialCoords, Directions direction, bool carryDirection = false)
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
    if (carryDirection)
        initialCoords.direction = direction;
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

class Maze
{
private:
    int sizeX, sizeY;
    std::vector<std::vector<Cell>> cells;

    // Generation
    std::stack<Coordinates> positionStack;
    std::uniform_int_distribution<u32> distribute2;
    std::uniform_int_distribution<u32> distribute3;
    engine generator;

    void printHorizontalLine(int length, int row, bool endline);

    // Generation
    bool generateWalls(Coordinates currentCoords, int visitedCount);
    bool setCellConnection(Coordinates coords, Directions direction);

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

    // Creating radom numbers distributors
    distribute2 = std::uniform_int_distribution<u32>(0, 1);
    distribute3 = std::uniform_int_distribution<u32>(0, 2);
}

bool Maze::setCellConnection(Coordinates coords, Directions direction)
{
    if (getCellByCoordinates(coords).setConnection(direction) == false)
        return false;

    // Add opposite connection to adjacent neighbors
    if (direction == top && coords.y > 0)
        getCellByCoordinates(coords.x, coords.y - 1).setConnection(bottom);
    else if (direction == bottom && coords.y < (sizeY - 1))
        getCellByCoordinates(coords.x, coords.y + 1).setConnection(top);
    else if (direction == left && coords.x > 0)
        getCellByCoordinates(coords.x - 1, coords.y).setConnection(right);
    else if (direction == right && coords.x < (sizeX - 1))
        getCellByCoordinates(coords.x + 1, coords.y).setConnection(left);
    else
        return false;
    return true;
}

void Maze::generateMaze()
{
    static const Coordinates startCoords(0, 0);
    generateWalls(startCoords, 0);

    // Clearing the cells from previous recursive backtracking
    for (std::vector<Cell> &row : cells)
        for (Cell &cell : row)
            cell.resetVisited();
}

bool Maze::generateWalls(Coordinates currentCoords, int visitedCount)
{
    if (visitedCount >= ((sizeX * sizeY) - 1))
        return true;

    getCellByCoordinates(currentCoords).setVisited();

    std::vector<Coordinates> availableNeighbors = {};

    if ((currentCoords.y - 1) >= 0 && !(getCellByCoordinates(CalculateCoordinatesFromDirection(currentCoords, top)).isVisited()))
        availableNeighbors.push_back(CalculateCoordinatesFromDirection(currentCoords, top, true));

    if ((currentCoords.x + 1) < (sizeX) && !(getCellByCoordinates(CalculateCoordinatesFromDirection(currentCoords, right)).isVisited()))
        availableNeighbors.push_back(CalculateCoordinatesFromDirection(currentCoords, right, true));

    if ((currentCoords.y + 1) < (sizeY) && !(getCellByCoordinates(CalculateCoordinatesFromDirection(currentCoords, bottom)).isVisited()))
        availableNeighbors.push_back(CalculateCoordinatesFromDirection(currentCoords, bottom, true));

    if ((currentCoords.x - 1) >= 0 && !(getCellByCoordinates(CalculateCoordinatesFromDirection(currentCoords, left)).isVisited()))
        availableNeighbors.push_back(CalculateCoordinatesFromDirection(currentCoords, left, true));

    // set cell connection
    Coordinates nextCellCoords;

    if (availableNeighbors.size() == 1)
        nextCellCoords = availableNeighbors.at(0);
    else if (availableNeighbors.size() == 2)
        nextCellCoords = availableNeighbors.at(distribute2(generator));
    else if (availableNeighbors.size() == 3)
        nextCellCoords = availableNeighbors.at(distribute2(generator));
    else
    {
        positionStack.pop();
        if (generateWalls(positionStack.top(), visitedCount))
            return true;
    }

    visitedCount++;
    positionStack.push(currentCoords);
    setCellConnection(currentCoords, nextCellCoords.direction);

    generateWalls(nextCellCoords, visitedCount);

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

        // Add junction at maze border
        if (x == (length - 1))
            std::cout << junction;
    }
    std::cout << std::endl;
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
        printHorizontalLine(sizeX, y);

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
}

Cell &Maze::getCellByCoordinates(Coordinates coords)
{
    return cells.at(coords.x).at(coords.y);
}

Cell &Maze::getCellByCoordinates(int x, int y)
{
    return cells.at(x).at(y);
}

// ========================

bool findPath(Maze &m, Coordinates from, Coordinates to)
{
    Coordinates currentNeighborCoords;
    std::vector<Directions> currentCellConnections;

    m.getCellByCoordinates(from)
        .setVisited();

    if (from == to)
        return true;

    currentCellConnections = m.getCellByCoordinates(from).getConnections();

    for (Directions &connection : currentCellConnections)
    {
        currentNeighborCoords = CalculateCoordinatesFromDirection(from, connection);

        if (m.getCellByCoordinates(currentNeighborCoords).isVisited() == false)
            if (findPath(m, currentNeighborCoords, to) == true)
                return true;
    }

    m.getCellByCoordinates(from).resetVisited();
    return false;
}

int main(int argc, char *argv[])
{
    u32 seed;
    int sizeY;
    int sizeX;

    try
    {
        if (argc < 3 || argc > 4)
            throw std::runtime_error("Lack of necessary arguments: [height] [width] [OPTIONAL seed]");

        sizeY = atoi(argv[1]);
        sizeX = atoi(argv[2]);

        if (sizeY <= 0 || sizeX <= 0)
            throw std::runtime_error("Bad size: width and height must be at lease 1 cell!");
    }
    catch (std::runtime_error &excpt)
    {
        std::cout << excpt.what() << std::endl;
        return -1;
    }

    if (argc == 3)
    {
        std::random_device os_seed;
        seed = os_seed();
    }
    else
        seed = atoi(argv[3]);

    Maze m(sizeX, sizeY, seed);
    m.generateMaze();

    if (findPath(m, Coordinates(0, 0), Coordinates(sizeX - 1, sizeY - 1)))
    {
        m.printMaze();
        return 0;
    }

    // Exit with abnormal exit code if a path could not be found
    return -1;
}
