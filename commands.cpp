#include "commands.h"
#include "directories.h"
#include "files.h"
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

void initializeItems(const string& path, Directory* currentDir) {}


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

string toLower(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return tolower(c); });
    return result;
}


bool compareByName(const systemItem* a, const systemItem* b) {
    if ((a->isDirectory() && b->isDirectory()) || (!a->isDirectory() && !b->isDirectory())) {
        return toLower(a->getName()) < toLower(b->getName());
    }
    return a->isDirectory();
}



void executeCommand(const string& command, vector<Directory*>& items, string& path) {
    istringstream iss(command);
    vector<string> tokens{ istream_iterator<string>{iss}, istream_iterator<string>{} };

    bool sortedBySize = false;
    bool sortedByName = false;

    string cmd = tokens[0];

    if (cmd == "dir") {
        __int64 totSize = 0;
        __int64 totFiles = 0;
        __int64 totDirs = 0;
        for (const auto& item : items) {
            cout << item->getDate() << "\t\t" << item->getSize() << "\t" << item->getName() << "\n";
            if (!item->isDirectory()) {
                totFiles++;
                totSize += stoi(item->getSize());
            }
            else {
                totDirs++;
            }
        }
        cout << "\t" << totFiles << " File(s)\t" << totSize << " total bytes\n";
        cout << "\t" << totDirs << " Dir(s) \n";

    }

    else if (cmd == "sortsize") {
        sort(items.begin(), items.end(), compareBySize);
        __int64 totSize = 0;
        __int64 totFiles = 0;
        __int64 totDirs = 0;
        for (const auto& item : items) {
            cout << item->getDate() << "\t\t" << item->getSize() << "\t" << item->getName() << "\n";
            if (!item->isDirectory()) {
                totFiles++;
                totSize += stoi(item->getSize());
            }
            else {
                totDirs++;
            }
        }
        cout << "   " << totFiles << " File(s)           " << totSize << " total bytes\n";
        cout << "   " << totDirs << " Dir(s) \n";
        sortedByName = false;
        sortedBySize = true;
    }

    else if (cmd == "sortname") {
        sort(items.begin(), items.end(), compareByName);
        __int64 totSize = 0;
        __int64 totFiles = 0;
        __int64 totDirs = 0;
        for (const auto& item : items) {
            cout << item->getDate() << "\t\t" << item->getSize() << "\t" << item->getName() << "\n";
            if (!item->isDirectory()) {
                totFiles++;
                totSize += stoi(item->getSize());
            }
            else {
                totDirs++;
            }
        }
        cout << "\t" << totFiles << " File(s)\t" << totSize << " total bytes\n";
        cout << "\t" << totDirs << " Dir(s) \n";
        sortedBySize = false;
        sortedByName = true;
    }

    else if (cmd == "cd") {
        if (tokens.size() < 2) {
            cout << "Usage: cd <directory>" << endl;
            return;
        }

        string targetDirectory = tokens[1];
        if (targetDirectory == "..") {
            if (filesystem::path(path).has_parent_path()) {
                path = filesystem::path(path).parent_path().string();
                cout << "Moved to parent directory." << endl;
            }
            else {
                cout << "Already at the root directory." << endl;
            }
        }
        if (targetDirectory == "\\") {
            path = ".";

            cout << "Moved to the root directory." << endl;
        }
        else {
            filesystem::path newPath = filesystem::path(path) / targetDirectory;
            if (filesystem::exists(newPath) && filesystem::is_directory(newPath)) {
                path = newPath.string();
                cout << "Moved to directory: " << path << endl;
            }
            else {
                cout << "Directory does not exist: " << targetDirectory << endl;
            }
        }
    }

    else if (cmd == "mkdir") {
        if (tokens.size() > 1) {
            string dirname = tokens[1];

            filesystem::path newDirPath = path + "\\" + dirname;
            if (!filesystem::exists(newDirPath)) {
                if (filesystem::create_directory(newDirPath)) {
                    cout << "Directory '" << dirname << "' created successfully." << endl;

                    Directory* newDirItem = new Directory(dirname, true, "<DIR>", "");
                    items.push_back(newDirItem);
                }
                else {
                    cout << "Failed to create directory '" << dirname << "'." << endl;
                }
            }
            else {
                cout << "Directory '" << dirname << "' already exists." << endl;
            }
        }
        else {
            cout << "Invalid command: mkdir requires a directory name." << endl;
        }
    }

    else if (cmd == "mkfile") {
        if (tokens.size() > 1) {
            string filename = tokens[1];

            filesystem::path newFilePath = filesystem::path(path) / filename;
            string fileSize = to_string(rand() % 1000 + 1);

            if (!filesystem::exists(newFilePath)) {
                ofstream newFile(newFilePath);
                if (newFile.is_open()) {
                    newFile.close();
                    cout << "File '" << filename << "' created successfully." << endl;
                    File* newFileItem = new File(filename, false, fileSize, "");
                    items.push_back(newFileItem);
                }
                else {
                    cout << "Failed to create file '" << filename << "'." << endl;
                }
            }
            else {
                cout << "File '" << filename << "' already exists." << endl;
            }
        }
        else {
            cout << "Invalid command: mkfile requires a file name." << endl;
        }
    }

    else if (cmd == "del") {
        if (tokens.size() > 1) {
            string name = tokens[1];

            filesystem::path itemPath = filesystem::path(path) / name;

            if (filesystem::exists(itemPath)) {
                error_code ec;
                if (filesystem::remove(itemPath, ec)) {
                    cout << "Deleted '" << name << "' successfully." << endl;

                    items.erase(remove_if(items.begin(), items.end(), [&](const systemItem* item) {
                        return item->getName() == name;
                        }), items.end());
                }
                else {
                    cout << "Failed to delete '" << name << "'. " << ec.message() << endl;
                }
            }
            else {
                cout << "File or directory '" << name << "' not found." << endl;
            }
        }
        else {
            cout << "Invalid command: del requires a file or directory name." << endl;
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
        cout << "Unknown command: " << cmd << endl;
    }

    if (cmd == "exit") {
        cout << "Exiting the program." << endl;
    }
    else if (!sortedBySize && !sortedByName) {

        initializeItems(path, items);
    }

}