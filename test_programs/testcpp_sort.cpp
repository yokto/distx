#include <iostream>
#include <vector>
#include <algorithm>


// Test function for std::sort with strings
void testStdSortStrings() {
    std::vector<std::string> strings = {"banana", "apple", "orange", "grape", "cherry"};

    // Display the original strings
    std::cout << "Original strings:" << std::endl;
    for (const auto& str : strings) {
        std::cout << str << std::endl;
    }
    std::cout << std::endl;

    // Sort the strings using std::sort
    std::sort(strings.begin(), strings.end());

    // Display the sorted strings
    std::cout << "Sorted strings:" << std::endl;
    for (const auto& str : strings) {
        std::cout << str << std::endl;
    }
}

int main() {
    std::cout << "Testing std::sort function with strings:" << std::endl;
    testStdSortStrings();
    return 0;
}

