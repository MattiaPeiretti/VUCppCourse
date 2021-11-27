// ============================================
//  Mattia Peiretti, November 2021
//  https://mattiapeiretti.com/
//  Assignment H4 - N of Queens
// ============================================

#define LOG(X) std::cout << X << std::endl;

#include <iostream>
#include <vector>

// Check to see if there is any conflict with previously placed queens
bool checkValidPosition(std::vector<std::vector<bool>> &board, int col, int row)
{
    //Horizontal check
    for (int x = 0; x < board.size(); x++)
    {
        if (board.at(row).at(x))
        {
            return false;
        }
    }
    //Vertical check
    for (int x = 0; x < board.size(); x++)
    {
        if (board.at(x).at(col))
        {
            return false;
        }
    }

    int d1BeginX;
    int d1BeginY;

    int d2BeginX;
    int d2BeginY;

    int boardSize = board.size() - 1;

    // Calculating Diagional 1
    d1BeginY = row - col;
    if (d1BeginY < 0)
    {
        d1BeginX = 0 - d1BeginY;
        d1BeginY = 0;
    }
    else
        d1BeginX = 0;

    // Calculating Diagional 2
    d2BeginX = col + row;
    if (d2BeginX > boardSize)
    { //                 Excess
        d2BeginY = (d2BeginX - boardSize);
        d2BeginX = boardSize;
    }
    else
        d2BeginY = 0;

    // first diagonal

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

bool placeQueens(int N, std::vector<std::vector<bool>> &board, int row)
{

        placeQueens(N, board, row + 1);
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

    chessBoard.at(0).at(0) = true;
    std::cout << checkValidPosition(chessBoard, 4, 4) << std::endl;
    return 0;
}