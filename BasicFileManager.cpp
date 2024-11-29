#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

using namespace std;

void listFiles(const fs::path& currentDir) {
    try {
        if (fs::exists(currentDir) && fs::is_directory(currentDir)) {
            cout << "Listing files in: " << currentDir << endl;
            for (const auto& entry : fs::directory_iterator(currentDir)) {
                cout << entry.path().filename().string() << (fs::is_directory(entry) ? " [DIR]" : " [FILE]") << endl;
            }
        } else {
            cout << "The directory does not exist!" << endl;
        }
    } catch (const fs::filesystem_error& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void changeDirectory(fs::path& currentDir, const string& path) {
    fs::path newDir = currentDir / path;
    try {
        if (fs::exists(newDir) && fs::is_directory(newDir)) {
            currentDir = newDir;
            cout << "Changed directory to: " << currentDir << endl;
        } else {
            cout << "Directory does not exist!" << endl;
        }
    } catch (const fs::filesystem_error& e) {
        cout << "Error: " << e.what() << endl;
    }
}


void createDirectory(const fs::path& currentDir, const string& dirName) {
    fs::path newDir = currentDir / dirName;
    try {
        if (fs::create_directory(newDir)) {
            cout << "Directory created: " << newDir << endl;
        } else {
            cout << "Failed to create directory (might already exist)." << endl;
        }
    } catch (const fs::filesystem_error& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void viewFile(const fs::path& filePath) {
    try {
        if (fs::exists(filePath) && fs::is_regular_file(filePath)) {
            ifstream file(filePath);
            stringstream buffer;
            buffer << file.rdbuf();
            cout << buffer.str() << endl;
        } else {
            cout << "File does not exist or is not a regular file!" << endl;
        }
    } catch (const std::ios_base::failure& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void copyFile(const fs::path& source, const fs::path& destination) {
    try {
        if (fs::exists(source) && fs::is_regular_file(source)) {
            fs::copy(source, destination, fs::copy_options::overwrite_existing);
            cout << "File copied from " << source << " to " << destination << endl;
        } else {
            cout << "Source file does not exist or is not a regular file!" << endl;
        }
    } catch (const fs::filesystem_error& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void moveFile(const fs::path& source, const fs::path& destination) {
    try {
        if (fs::exists(source) && fs::is_regular_file(source)) {
            fs::rename(source, destination);
            cout << "File moved from " << source << " to " << destination << endl;
        } else {
            cout << "Source file does not exist or is not a regular file!" << endl;
        }
    } catch (const fs::filesystem_error& e) {
        cout << "Error: " << e.what() << endl;
    }
}

int main() {
    fs::path currentDir = fs::current_path(); 
    string command;

    cout << "Welcome to the C++ Command Line File Manager!" << endl;
    cout << "Available commands: list, cd <dir>, create <dir>, view <file>, copy <src> <dest>, move <src> <dest>, exit" << endl;

    while (true) {
        cout << "\nCurrent Directory: " << currentDir << endl;
        cout << "Enter command: ";
        getline(cin, command);

        stringstream ss(command);
        vector<string> words;
        string word;
        while (ss >> word) {
            words.push_back(word);
        }
        
        if (words[0] == "list") {
            listFiles(currentDir);
        } else if (words[0] == "cd" && words.size() == 2) {
            changeDirectory(currentDir, words[1]);
        } else if (words[0] == "create" && words.size() == 2) {
            createDirectory(currentDir, words[1]);
        } else if (words[0] == "view" && words.size() == 2) {
            viewFile(currentDir / words[1]);
        } else if (words[0] == "copy" && words.size() == 3) {
            copyFile(currentDir / words[1], currentDir / words[2]);
        } else if (words[0] == "move" && words.size() == 3) {
            moveFile(currentDir / words[1], currentDir / words[2]);
        } else if (words[0] == "exit") {
            cout << "Exiting file manager. Goodbye!" << endl;
            break;
        } else {
            cout << "Invalid command. Try again." << endl;
        }
    }

    return 0;
}