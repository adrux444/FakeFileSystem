#include "commands.h"
#include <iostream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include "systemItem.h"
#include <fstream>
#include <cstdlib>


using namespace std;

void initializeItems(const filesystem::path& rootPath, Directory& rootDirectory);


bool compareBySize(const systemItem* a, const systemItem* b) {
    if (a->isDirectory() && !b->isDirectory()) {
        return true;
    }
    else if (!a->isDirectory() && b->isDirectory()) {
        return false;
    }

    if (a->isDirectory() && b->isDirectory()) {
        return false;
    }
    else {
        int sizeA = stoi(a->getSize());
        int sizeB = stoi(b->getSize());
        return sizeA < sizeB;
    }

    return (a->getSize() < b->getSize());
}

std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return result;
}


bool compareByName(const systemItem* a, const systemItem* b) {
    if ((a->isDirectory() && b->isDirectory()) || (!a->isDirectory() && !b->isDirectory())) {
        return toLower(a->getName()) < toLower(b->getName());
    }
    return a->isDirectory(); 
}



void executeCommand(const std::string& command, Directory items, std::string& path) {
    std::istringstream iss(command);
    std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{} };

    bool sortedBySize = false;
    bool sortedByName = false;

    std::string cmd = tokens[0];

    auto& subDirs = items.getSubDirectories();
    auto& files = items.getFiles();

    if (cmd == "dir") {
        __int64 totSize = 0;
        __int64 totFiles = 0;
        __int64 totDirs = 0;
        for (const auto& dir : subDirs) {
            cout << dir->getDate() << "\t\t" << dir->getSize() << "\t" << dir->getName() << "\n";
            totDirs++;
        }

        for (const auto& file : files) {
            cout << file->getDate() << "\t\t" << file->getSize() << "\t" << file->getName() << "\n";
            totFiles++;
            totSize += stoi(file->getSize());
        }
        cout << "\t" << totFiles << " File(s)\t" << totSize << " total bytes\n";
        cout << "\t" << totDirs << " Dir(s) \n";

    }

    else if (cmd == "sortsize") {
        std::sort(subDirs.begin(), subDirs.end(), compareBySize);
        std::sort(files.begin(), files.end(), compareBySize);
        __int64 totSize = 0;
        __int64 totFiles = 0;
        __int64 totDirs = 0;

        for (const auto& dir : subDirs) {
            cout << dir->getDate() << "\t\t" << dir->getSize() << "\t" << dir->getName() << "\n";
            totDirs++;
        }

        for (const auto& file : files) {
            cout << file->getDate() << "\t\t" << file->getSize() << "\t" << file->getName() << "\n";
            totFiles++;
            totSize += stoi(file->getSize());
        }
        
        cout << "   " << totFiles << " File(s)           " << totSize << " total bytes\n";
        cout << "   " << totDirs << " Dir(s) \n";
        sortedByName = false;
        sortedBySize = true;
    }

    else if (cmd == "sortname") {
        std::sort(subDirs.begin(), subDirs.end(), compareByName);
        std::sort(files.begin(), files.end(), compareByName);
        __int64 totSize = 0;
        __int64 totFiles = 0;
        __int64 totDirs = 0;

        for (const auto& dir : subDirs) {
            cout << dir->getDate() << "\t\t" << dir->getSize() << "\t" << dir->getName() << "\n";
            totDirs++;
        }

        for (const auto& file : files) {
            cout << file->getDate() << "\t\t" << file->getSize() << "\t" << file->getName() << "\n";
            totFiles++;
            totSize += stoi(file->getSize());
        }

        cout << "   " << totFiles << " File(s)           " << totSize << " total bytes\n";
        cout << "   " << totDirs << " Dir(s) \n";
        sortedByName = false;
        sortedBySize = true;
    }

    else if (cmd == "cd") {
        if (tokens.size() < 2) {
            cout << "Usage: cd <directory>" << endl;
            return;
        }

        string targetDirectory = tokens[1];
        filesystem::path newPath;

        if (targetDirectory == "..") {
            if (filesystem::path(path).has_parent_path()) {
                newPath = filesystem::path(path).parent_path();
                cout << "Moved to parent directory." << endl;
            }
            else {
                cout << "Already at the root directory." << endl;
                return;
            }
        }
        else if (targetDirectory == "\\") {
            newPath = "."; // Root directory
            cout << "Moved to the root directory." << endl;
        }
        else {
            newPath = filesystem::absolute(filesystem::path(path) / targetDirectory);
        }

        if (filesystem::exists(newPath) && filesystem::is_directory(newPath)) {
            path = newPath.string();
            cout << "Moved to directory: " << path << endl;
        }
        else {
            cout << "Directory does not exist: " << targetDirectory << endl;
        }
    }


    else if (cmd == "mkdir") {
        if (tokens.size() > 1) {
            std::string dirname = tokens[1];

            std::filesystem::path newDirPath = path + "\\" + dirname;
            if (!std::filesystem::exists(newDirPath)) {
                if (std::filesystem::create_directory(newDirPath)) {
                    std::cout << "Directory '" << dirname << "' created successfully." << std::endl;

                    Directory* newDirItem = new Directory(dirname, true, "<DIR>", "");
                    items.addSubDirectory(newDirItem); // Add the new directory item to the vector
                }
                else {
                    std::cout << "Failed to create directory '" << dirname << "'." << std::endl;
                }
            }
            else {
                std::cout << "Directory '" << dirname << "' already exists." << std::endl;
            }
        }
        else {
            std::cout << "Invalid command: mkdir requires a directory name." << std::endl;
        }
    }
    else if (cmd == "mkfile") {
        if (tokens.size() > 1) {
            std::string filename = tokens[1];

            std::filesystem::path newFilePath = std::filesystem::path(path) / filename;
            std::string fileSize = std::to_string(rand() % 1000 + 1);

            if (!std::filesystem::exists(newFilePath)) {
                std::ofstream newFile(newFilePath);
                if (newFile.is_open()) {
                    newFile.close();
                    std::cout << "File '" << filename << "' created successfully." << std::endl;
                    File* newFileItem = new File(filename, false, fileSize, "");
                    items.addFile(newFileItem); // Add the new file item to the vector
                }
                else {
                    std::cout << "Failed to create file '" << filename << "'." << std::endl;
                }
            }
            else {
                std::cout << "File '" << filename << "' already exists." << std::endl;
            }
        }
        else {
            std::cout << "Invalid command: mkfile requires a file name." << std::endl;
        }
    }




    else if (cmd == "del") {
        if (tokens.size() > 1) {
            std::string name = tokens[1];

            // Check if the item exists in the current directory
            bool found = false;
            auto& subDirs = items.getSubDirectories(); // Get non-const reference to subDirectories vector
            auto& files = items.getFiles(); // Get non-const reference to files vector

            // Iterate over subdirectories
            auto subDirIt = subDirs.begin();
            while (subDirIt != subDirs.end()) {
                if ((*subDirIt)->getName() == name) {
                    delete* subDirIt; // Delete the directory object
                    subDirIt = subDirs.erase(subDirIt); // Erase from the vector
                    found = true;
                }
                else {
                    ++subDirIt;
                }
            }

            // If not found in subdirectories, search in files
            if (!found) {
                auto fileIt = files.begin();
                while (fileIt != files.end()) {
                    if ((*fileIt)->getName() == name) {
                        delete* fileIt; // Delete the file object
                        fileIt = files.erase(fileIt); // Erase from the vector
                        found = true;
                    }
                    else {
                        ++fileIt;
                    }
                }
            }

            // Check if the item was found and deleted
            if (found) {
                std::filesystem::path itemPath = std::filesystem::path(path) / name;
                if (std::filesystem::exists(itemPath)) {
                    std::error_code ec;
                    if (std::filesystem::remove(itemPath, ec)) {
                        std::cout << "Deleted '" << name << "' successfully." << std::endl;
                    }
                    else {
                        std::cout << "Failed to delete '" << name << "'. " << ec.message() << std::endl;
                    }
                }
                else {
                    std::cout << "Failed to find path for '" << name << "'." << std::endl;
                }
            }
            else {
                std::cout << "File or directory '" << name << "' not found." << std::endl;
            }
        }
        else {
            std::cout << "Invalid command: del requires a file or directory name." << std::endl;
        }
        }



    else if (cmd == "help") {
        cout << "Commands:\n"
            << "dir - list the files and directories in the current directory\n"
            << "sortsize - sort the files and directories by size\n"
            << "sortname - sort the files and directories by name\n"
            << "cd <directory> - change the current directory\n"
            << "mkdir <directory> - create a new directory\n"
            << "mkfile <filename> - create a new file\n"
            << "del <filename> - delete a file or directory\n"
            << "help - display this help message\n"
            << "exit - exit the program\n";
    }
    else if (cmd == "exit") {
    }
    else {
        std::cout << "Unknown command: " << cmd << std::endl;
    }

    if (cmd == "exit") {
       cout << "Exiting the program." << endl;
    }
    else if (!sortedBySize && !sortedByName) {

        initializeItems(path, items);
    }

}