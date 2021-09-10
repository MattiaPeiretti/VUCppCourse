#include <iostream>

int findNextCollatzSequence(int currentNumberOfSequence) {
    // Checking if the first number is even
    if (currentNumberOfSequence%2 == 0) currentNumberOfSequence = currentNumberOfSequence / 2;
    // Number is odd
    else currentNumberOfSequence = (currentNumberOfSequence * 3) + 1;
    
    return currentNumberOfSequence;
}

int main() {
    const int MAX_NUMBER_OF_ONES = 3;
    int currentNumberOfSequence;
    int oneCounter = 0;

    std::cout << "Enter the first number of the sequence: " << std::endl;
    std::cin >> currentNumberOfSequence;
    if(currentNumberOfSequence < 0) {
        std::cout << "Illegal, only positive numbers are allowed!";
        return -1;
    }

    std::cout << currentNumberOfSequence << " ";

    if (currentNumberOfSequence == 1) oneCounter++;

    while (oneCounter < MAX_NUMBER_OF_ONES) {
        currentNumberOfSequence = findNextCollatzSequence(currentNumberOfSequence);
        std::cout << currentNumberOfSequence << " ";
        if (currentNumberOfSequence == 1)
            oneCounter++;
    }

    std::cout << "..." << std::endl;

    return 0;
}