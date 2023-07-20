#include <iostream>
#include <vector>

int testVector() {
    std::vector<int> myVector;

    // Test empty vector
    if (!myVector.empty() || myVector.size() != 0) {
        std::cout << "Test 1: Failed (Empty vector check)\n";
        return -1;
    }

    // Test push_back and size
    for (int i = 1; i <= 5; ++i) {
        myVector.push_back(i * 10);
    }
    if (myVector.empty() || myVector.size() != 5) {
        std::cout << "Test 2: Failed (Push_back and size check)\n";
        return -1;
    }

    // Test element access
    if (myVector[0] != 10 || myVector[2] != 30 || myVector[4] != 50) {
        std::cout << "Test 3: Failed (Element access)\n";
        return -1;
    }

    // Test pop_back and size after popping
    myVector.pop_back();
    if (myVector.size() != 4 || myVector.back() != 40) {
        std::cout << "Test 4: Failed (Pop_back and size after popping)\n";
        return -1;
    }

    // Test insert and size after insertion
    myVector.insert(myVector.begin(), 0);
    if (myVector.size() != 5 || myVector.front() != 0) {
        std::cout << "Test 5: Failed (Insert and size after insertion)\n";
        return -1;
    }

    // Test erase and size after erasing
    myVector.erase(myVector.begin() + 2);
    if (myVector.size() != 4 || myVector[2] != 30 || myVector[3] != 50) {
        std::cout << "Test 6: Failed (Erase and size after erasing)\n";
        return -1;
    }

    std::cout << "All tests passed successfully!\n";
    return 0;
}

int main() {
    int result = testVector();
    return result;
}
