// ============================================
//  Mattia Peiretti, November 2021
//  https://mattiapeiretti.com/
//  Assignment H4 - N of Queens
// ============================================

#define LOG(X) std::cout << X << std::endl;

#include <iostream>
#include <string>
#include <vector>

void printChessBoard(std::vector<std::vector<bool>> &board)
{
    static const std::string emptyCell = ".";
    static const std::string queen = "Q";
    static const std::string spacer = " ";

    for (int y = 0; y < board.size(); y++)
    {
        for (int x = 0; x < board.size(); x++)
        {
            if (board.at(y).at(x))
                std::cout << queen << spacer;
            else
                std::cout << emptyCell << spacer;
        }
        std::cout << std::endl;
    }
}

// Check to see if there is any conflict with previously placed queens
bool checkValidPosition(std::vector<std::vector<bool>> &board, int col, int row)
{
    //Checking horizontally and vertically
    for (int x = 0; x < board.size(); x++)
    {
        if (board.at(row).at(x))
            return false;
        if (board.at(x).at(col))
            return false;
    }

    int boardSize = board.size() - 1;

    // Calculating top point of the Diagonal 1
    int d1BeginX;
    int d1BeginY;

    d1BeginY = row - col;
    if (d1BeginY < 0)
    {
        d1BeginX = 0 - d1BeginY;
        d1BeginY = 0;
    }
    else
        d1BeginX = 0;

    // Calculating top point of the Diagonal 2
    int d2BeginX;
    int d2BeginY;

    d2BeginX = col + row;
    if (d2BeginX > boardSize)
    { //                 Excess
        d2BeginY = (d2BeginX - boardSize);
        d2BeginX = boardSize;
    }
    else
        d2BeginY = 0;

    int currentX1 = d1BeginX, currentY1 = d1BeginY;
    int currentX2 = d2BeginX, currentY2 = d2BeginY;

    while (true)
    {
        if (currentX1 >= board.size(), currentY1 >= board.size())
            break;
        if (currentX2 >= board.size(), currentY2 >= board.size())
            break;

        if (board.at(currentY1).at(currentX1))
        {
            return false;
        }
        if (board.at(currentY2).at(currentX2))
        {
            return false;
        }

        currentX1++;
        currentY1++;
        currentX2--;
        currentY2--;
    }

    return true;
}

int placedQueens = 0;
bool placeQueens(int N, std::vector<std::vector<bool>> &board, int row)
{
    if (row >= N)
        return 1;
    printChessBoard(board);
    std::cin.get();

    for (int col = 0; col < board.size(); col++)
    {
        if (checkValidPosition(board, col, row))
        {
            board.at(row).at(col) = true;
            if (placeQueens(N, board, row + 1))
                return true;
            board.at(row).at(col) = false;
        }
    }

    return 0;
}

int parseUserInput()
{
    int numberOfQueens;
    std::cout << "How many queens to place on the board? ";
    std::cin >> numberOfQueens;
    if (std::cin.fail())
        throw std::runtime_error("error: invalid input");
    if (!numberOfQueens >= 1)
        throw std::runtime_error("error: invalid input");
    return numberOfQueens;
}

int main()
{

    int numberOfQueens = 0;
    int chessBoardSize = 0;
    bool result;

    try
    {
        chessBoardSize = numberOfQueens = parseUserInput();
    }
    catch (std::runtime_error &excpt)
    {
        std::cout << excpt.what() << std::endl;
        return -1;
    }

    std::vector<std::vector<bool>> chessBoard(chessBoardSize, std::vector<bool>(chessBoardSize, false));
    // result = placeQueens(numberOfQueens, chessBoard, 1);

    placeQueens(numberOfQueens, chessBoard, 0);
    printChessBoard(chessBoard);

    return 0;
}