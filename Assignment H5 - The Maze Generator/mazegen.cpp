#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

enum directions
{
    top = 0,
    right,
    bottom,
    left
};

// ========================

class Cell
{
private:
    std::vector<directions> connections;
    bool isVisited = false;

public:
    Cell(){};
    std::vector<directions> getConnections();
    bool setConnection(directions direction);
    bool isConnectedTo(directions direction);
    bool setVisited();
    bool getVisited() { return false; }
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

// ========================

class Maze
{
private:
    std::vector<std::vector<Cell>> cells;
    int sizeX;
    int sizeY;
    void printHorizontalLine(int length, int row, bool endline);
    Cell &getCellByCoordinates(int x, int y);

public:
    Maze(int _sizeX, int _sizeY);
    bool setCellConnection(int x, int y, directions direction);
    void generateMaze();
    const void printMaze();
};

bool Maze::setCellConnection(int x, int y, directions direction)
{
    Cell currentCell = getCellByCoordinates(x, y);
    if (!currentCell.setConnection(direction))
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
Maze::Maze(int _sizeX, int _sizeY)
{
    cells = std::vector<std::vector<Cell>>(_sizeX, std::vector<Cell>(_sizeY));

    sizeX = _sizeX;
    sizeY = _sizeY;
}

void Maze::generateMaze()
{
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
        else if (currentCell.isConnectedTo(bottom) && row > 0)
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
    const static std::string filledCell = " . ";
    const static std::string emptyCell = "   ";
    std::string cellContent;

    for (int y = 0; y < sizeY; y++)
    {

        // will print slightly different line on final X loop
        printHorizontalLine(sizeX, y);
        std::cout << std::endl;

        for (int x = 0; x < sizeX; x++)
        {
            cellContent = cells.at(x).at(y).getVisited() ? filledCell : emptyCell;
            std::cout << verticalWall << cellContent;

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

    Maze m(5, 5);
    std::cout << m.setCellConnection(1, 1, bottom);
    std::cout << m.setCellConnection(1, 4, top);
    std::cout << std::endl;
    m.printMaze();
}
