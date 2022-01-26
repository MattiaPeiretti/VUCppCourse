#include <iostream>
#include <vector>
#include "vector.h"

class TestFailed{
    public:
        TestFailed(int seq){
            sequenceNumber = seq;
        }
        int sequenceNumber;
};

int testNumber = 0;

#define check(CALL) { testNumber++; if ( !(CALL) ) throw TestFailed(testNumber); }

#define checkException(CALL){ \
    testNumber++; \
    bool exceptionRaised = false; \
    try{ \
        CALL; \
    }catch(std::out_of_range& o){ \
        exceptionRaised = true; \
    }catch (...){} \
    if ( !exceptionRaised ) throw TestFailed(testNumber); \
}

template <typename T> void pushBack(Vector<T>& vector, T& value){
    unsigned int oldSize = vector.size();
    vector.resize(oldSize+1);
    vector.at(oldSize) = value;
}

template <typename T> void popBack(Vector<T>& vector){
    unsigned int oldSize = vector.size();
    vector.resize(oldSize-1);
}

int main(){
    try{
        checkException(Vector<int> foo(-2)); // test 1

        Vector<std::string>vs(1);
        checkException(vs.resize(-7));

        Vector<char> v(0);
        std::vector<char> testData = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

        check(v.size() == 0);
        checkException(v.at(0));

        pushBack(v,testData.at(0));
        check(v.size() == 1); // test 5
        check(v.at(0) == testData.at(0));
        checkException(v.at(1));
 
        pushBack(v,testData.at(1));
        check(v.size() == 2);
        check(v.at(1) == testData.at(1));
        check(v.at(0) == testData.at(0)); // test 10

        popBack(v);
        popBack(v);
        check(v.size()==0);

        checkException(popBack(v));
        pushBack(v,testData.at(2));
        pushBack(v,testData.at(3));
        pushBack(v,testData.at(4));
        check(v.size()==3);
        check(v.at(0)==testData.at(2));
        check(v.at(1)==testData.at(3)); // test 15
        check(v.at(2)==testData.at(4));
        checkException(v.at(3));

        v.at(0) = testData.at(4);
        check(v.at(0)==testData.at(4));
        checkException(v.at(-5));  // test 18
        
        std::cout << "All tests passed!\n";
    }
    catch(TestFailed& tf){
        std::cerr << "Test number " << tf.sequenceNumber << " failed.\n";
        return 1;
    }
    catch(...){
        std::cerr << "an unexpected exception occured\n";
        std::cerr << "Tests passed so far: " << testNumber << std::endl;
        return 2;
    }
    return 0;
}