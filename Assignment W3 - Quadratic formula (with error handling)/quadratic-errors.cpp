// ========================================================
//  Mattia Peiretti, November 2021
//  https://mattiapeiretti.com/
//  Assignment W3 - Quadratic formula with error handling
// ========================================================

#include <iostream>
#include <sstream>
#include <string>
#include <limits>
#include <cmath>
#include <vector>

#define LOG(X) std::cout << X << std::endl;

static const std::string ERRORMSG_MALFORMED_INPUT = "An error occurred: Malformed user input";
static const std::string ERRORMSG_A_ZERO_INPUT = "An error occurred: a must not be zero";

struct QuadraticFormulaConstants
{
    double a;
    double b;
    double c;
};

QuadraticFormulaConstants parseUserInput()
{
    std::string queryString;
    QuadraticFormulaConstants constants;
    std::stringstream ss;
    double a, b, c;
    a = b = c = std::numeric_limits<double>::quiet_NaN();

    std::getline(std::cin, queryString);

    //Checking that the user input contains only numbers
    if (queryString.find_first_not_of("0123456789. ") != std::string::npos)
        throw std::runtime_error(ERRORMSG_MALFORMED_INPUT);

    // Extracting the numbers from the string
    ss.str(queryString);
    ss >> a >> b >> c;

    // Checking that all necessary values have been inputted
    if (std::isnan(a) or std::isnan(b) or std::isnan(c))
        throw std::runtime_error(ERRORMSG_MALFORMED_INPUT);

    // Casting the obtained variables to integer
    // and checking if they match they non casted origin.
    if ((int)a != a and (int) b != b and (int) c != c)
        throw std::runtime_error(ERRORMSG_MALFORMED_INPUT);

    if (a == 0)
        throw std::runtime_error(ERRORMSG_A_ZERO_INPUT);

    constants.a = a;
    constants.b = b;
    constants.c = c;

    return constants;
}

double calculateDiscriminant(QuadraticFormulaConstants constants)
{
    // b^2 - 4ac
    return pow(constants.b, 2.0) - 4.0 * constants.a * constants.c;
}

int calculatePossibleResults(double D)
{
    if (D > 0)
        return 2;
    else if (D == 0)
        return 1;
    return 0;
}

std::vector<double> calculateQuadraticFormula(double a, double b, double c, double D)
{
    if (calculatePossibleResults(D) == 2)
        return {((-1.0 * b) + sqrt(D)) / (2.0 * a), ((-1.0 * b) - sqrt(D)) / (2.0 * a)};
    else if (calculatePossibleResults(D) == 1)
        return {-1 * (b / (a * 2.0))};
    return {};
}

//TODO: catch Fn: parseUserInput errors and print them on screen.
int main()
{
    double D;
    std::vector<double> solutions;
    QuadraticFormulaConstants constants;

    std::cout << "Please enter the values of a, b, and c: ";

    try
    {
        constants = parseUserInput();
    }
    catch (std::runtime_error &excpt)
    {
        std::cout << excpt.what() << std::endl;
        return -1;
    }

    std::cout << std::endl;

    // Calculate and check the discriminant
    D = calculateDiscriminant(constants);

    // Calculate the possible routes
    solutions = calculateQuadraticFormula(constants.a, constants.b, constants.c, D);
    if (calculatePossibleResults(D) == 2)
        std::cout << "There are 2 solutions." << std::endl
                  << std::endl
                  << "The solutions are: " << solutions.at(0)
                  << " and "
                  << solutions.at(1);
    else if (calculatePossibleResults(D) == 1)
        std::cout << "There is 1 solution." << std::endl
                  << std::endl
                  << "The solution is: " << solutions.at(0);
    else
        std::cout << "There is no solution.";
    std::cout << std::endl;

    return 0;
}