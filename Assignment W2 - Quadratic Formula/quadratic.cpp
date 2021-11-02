#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <vector>
#define LOG(X) std::cout << X << std::endl;

double calculateDiscriminant(double a, double b, double c) {
    // b^2 - 4ac
    return pow(b, 2.0) - (4.0 * a * c);   
}

int calculatePossibleResults(double D) {
    if (D>0)
        return 2;
    else if (D == 0)    
        return 1;
    else
        return 0;
}

std::vector<double> calculateQuadraticFormula(double a, double b, double c, double D) {
    if (calculatePossibleResults(D)==2) return {((-1.0 * b) + sqrt(D))/(2.0*a), ((-1.0 * b) - sqrt(D))/(2.0*a)};
    else if (calculatePossibleResults(D)==1) return {-1 * (b / (a * 2.0))};
    return {};
}

int main() {
    double a, b, c, D;
    std::string queryString;
    std::vector<double> solutions;

    std::cout << "Please enter the values of a, b, and c: ";
    std::getline(std::cin, queryString);

    // Extracting the numbers from the string
    std::stringstream ss;
    ss.str(queryString);
    ss >> a >> b >> c;

    // Calculate and check the discriminant
    D = calculateDiscriminant(a, b, c);

    solutions = calculateQuadraticFormula(a, b, c, D);
    LOG(D);
    if (calculatePossibleResults(D) == 2)
        std::cout << "There are 2 solutions." << std::endl  
                  << "The solutions are: " << solutions.at(0),
            solutions.at(1);
    else if (calculatePossibleResults(D)==1)
        std::cout << "There are 1 solution." << std::endl
                  << "The solution is: " << solutions.at(0);
    else
        std::cout << "There is no solution.";

    return 0;
}