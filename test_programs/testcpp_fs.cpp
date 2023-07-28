#include <iostream>
#include <stdio.h>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

const std::string test_folder = "./testcpp_fs_files";

int createFolder() {
    try {
	fs::remove_all(test_folder);
        fs::create_directory(test_folder);
        std::cout << "Created directory: " << test_folder << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error creating directory: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}

int testFileFunctionality() {
    std::string test_file = test_folder + "/test_file.txt";
    std::string new_test_file = test_folder + "/new_test_file.txt";
    
    // Create a file and write data
    std::ofstream outfile(test_file);
    if (!outfile.is_open()) {
        std::cerr << "Error creating file: " << test_file << std::endl;
        return -1;
    }
    
    outfile << "Hello, this is a test file!" << std::endl;
    outfile.close();
    std::cout << "Created file: " << test_file << std::endl;
    
    // Read and display data from the file
    std::ifstream infile(test_file);
    if (!infile.is_open()) {
        std::cerr << "Error opening file: " << test_file << std::endl;
        return -1;
    }
    
    std::string line;
    while (std::getline(infile, line)) {
        std::cout << line << std::endl;
    }
    infile.close();
    
    // Rename (move) the file
    try {
        fs::rename(test_file, new_test_file);
        std::cout << "Moved file: " << new_test_file << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error moving file: " << e.what() << std::endl;
        return -1;
    }
    
    // Delete the file
    try {
        fs::remove(new_test_file);
        std::cout << "Deleted file: " << new_test_file << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error deleting file: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}

int testDirectoryFunctionality() {
    std::string test_directory = test_folder + "/test_directory";
    std::string new_test_directory = test_folder + "/new_test_directory";
    
    // Create a directory
    try {
        fs::create_directory(test_directory);
        std::cout << "Created directory: " << test_directory << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error creating directory: " << e.what() << std::endl;
        return -1;
    }
    
    // Rename (move) the directory
    try {
        fs::rename(test_directory, new_test_directory);
        std::cout << "Moved directory: " << new_test_directory << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error moving directory: " << e.what() << std::endl;
        return -1;
    }
    
    // Delete the directory
    try {
        fs::remove_all(new_test_directory);
        std::cout << "Deleted directory: " << new_test_directory << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error deleting directory: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}

void printFileType(const std::string& filename) {
    if (fs::is_regular_file(filename)) {
        std::cout << "Regular file" << std::endl;
    } else if (fs::is_directory(filename)) {
        std::cout << "Directory" << std::endl;
    } else {
        std::cout << "Unknown file type" << std::endl;
    }
}

int main() {
    printFileType(".");

    if (createFolder() != 0) {
        return -1;
    }
    
    std::cout << "Testing file functionality:" << std::endl;
    if (testFileFunctionality() != 0) {
        return -1;
    }
    
    std::cout << "\nTesting directory functionality:" << std::endl;
    if (testDirectoryFunctionality() != 0) {
        return -1;
    }
    
    return 0;
}

