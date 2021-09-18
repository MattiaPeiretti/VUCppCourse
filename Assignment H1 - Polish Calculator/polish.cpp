#include <iostream>
#include <string>
#include <sstream>

int main() {

    std::string currentQuery = "", currentQueryNumbers = "";
    char currentOperation;
    float firstOperand, secondOperand, operationResult;

    while (true) {
        std::cout << "Enter the expression: ";
        std::getline(std::cin, currentQuery);

        // Getting the operation symbol 
        currentOperation = currentQuery.substr(0, currentQuery.find(" "))[0];
        currentQueryNumbers = currentQuery.substr(currentQuery.find(" ") + 1, std::string::npos);

        // Getting operators
        std::stringstream ss;
        ss.str(currentQueryNumbers);
        ss >> firstOperand >> secondOperand ;

        switch(currentOperation) {
            case '+':
                operationResult = firstOperand + secondOperand;
                break;
            
            case '-':
                operationResult = firstOperand - secondOperand;
                break;
            
            case '*':
                operationResult = firstOperand * secondOperand;
                break;
            
            case '/':
                operationResult = firstOperand / secondOperand;
                break;
            
            case 'q':
                return 0;
                break;
        }

        std::cout << firstOperand << " " << currentOperation << " " << secondOperand << " = " << operationResult <<std::endl;
    }

    return 0;
}