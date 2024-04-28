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
#include <set>


using namespace std;

void initializeItems(const string& path, Directory* currentDir);

string getDateTime() {
    
    time_t currentTime;
    time(&currentTime);
    tm localTime;
    localtime_s(&localTime, &currentTime);

    stringstream dateTime;
    dateTime << put_time(&localTime, "%d/%m/%Y %H:%M");

    return dateTime.str();
}

void sortSize(vector<systemItem*>& items) {
    set<string> uniqueNames;
    vector<systemItem*> uniqueItems;

    for (auto item : items) {
        if (uniqueNames.find(item->getName()) == uniqueNames.end()) {
            uniqueNames.insert(item->getName());
            uniqueItems.push_back(item);
        }
    }

    sort(uniqueItems.begin(), uniqueItems.end(), [](const systemItem* a, const systemItem* b) {
        if (a->isDirectory() && !b->isDirectory()) {
            return true;
        }
        else if (!a->isDirectory() && b->isDirectory()) {
            return false;
        }
        else {
            if (a->isDirectory()) {
                return false;
            }
            else {
                int sizeA = stoi(a->getSize());
                int sizeB = stoi(b->getSize());
                return sizeA < sizeB;
            }
        }
        });

    items = uniqueItems;

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
}





string toLower(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return tolower(c); });
    return result;
}


void sortName(vector<systemItem*>& items) {
    set<string> uniqueNames;
    vector<systemItem*> uniqueItems;

    //stop program duplicating items
    for (auto item : items) {
        if (uniqueNames.find(item->getName()) == uniqueNames.end()) {
            uniqueNames.insert(item->getName());
            uniqueItems.push_back(item);
        }
    }

    sort(uniqueItems.begin(), uniqueItems.end(), [](const systemItem* a, const systemItem* b) {
        if ((a->isDirectory() && b->isDirectory()) || (!a->isDirectory() && !b->isDirectory())) {
            return toLower(a->getName()) < toLower(b->getName());
        }
        return a->isDirectory();
        });

    items = uniqueItems;

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
}


void executeCommand(const string& command, vector<systemItem*>& items, string& path, Directory*& currDir, systemItem*& constant) {
    istringstream iss(command);
    vector<string> tokens{ istream_iterator<string>{iss}, istream_iterator<string>{} };

    currDir->setItems(items);
    bool sortedBySize = false;
    bool sortedByName = false;

    string cmd = tokens[0];

    if (cmd == "dir") {
        __int64 totSize = 0;
        __int64 totFiles = 0;
        __int64 totDirs = 0;
        items.clear();
        items = currDir->getItems();
        set<string> uniqueNames;
        vector<systemItem*> uniqueItems;

        for (auto item : items) {
            if (uniqueNames.find(item->getName()) == uniqueNames.end()) {
                uniqueNames.insert(item->getName());
                uniqueItems.push_back(item);
            }

        }
        for (const auto& item : uniqueItems) {
            cout << *item;
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
        vector<systemItem*> items = currDir->getItems();
        sortSize(items);
        sortedByName = false;
        sortedBySize = true;
    }


    else if (cmd == "sortname") {
        vector<systemItem*> items = currDir->getItems();
        sortName(items);
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
            Directory* parentDir = currDir->getParentDir();
            if (parentDir) {
                path = parentDir->getPath();
                currDir = parentDir;
            }
            else {
                cout << "Cannot move further up from the root directory." << endl;
            }
        }
        if (targetDirectory == "\\") {

            while (currDir->getParentDir() != nullptr)
            {
                currDir = currDir->getParentDir();
                path = currDir->getPath();
            }
        }
        else {
            bool found = false;
            for (auto subDir : currDir->getSubDirectories()) {
                if (toLower(subDir->getName()) == toLower(targetDirectory)) {
                    path = subDir->getPath();
                    currDir = subDir;
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << "Directory does not exist: " << targetDirectory << endl;
            }
        }
    }



    else if (cmd == "mkdir") {
        if (tokens.size() > 1) {
            string dirname = tokens[1];
            bool newDir = true;

            for (const auto& item : currDir->getItems()) {
                if (item->isDirectory() && item->getName() == dirname) {
                    newDir = false;
                    break;
                }
            }

            if (newDir) {
                Directory* newDirItem = new Directory(dirname, true, "<DIR>", getDateTime());
                currDir->addSubDirectory(newDirItem);
                cout << "Directory '" << dirname << "' created successfully." << endl;
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

            bool newFile = true;
            for (const auto& item : currDir->getItems()) {
                if (!item->isDirectory() && item->getName() == filename) {
                    newFile = false;
                    break;
                }
            }

            if (newFile) {
                string fileSize = to_string(rand() % 1000 + 1);
                File* newFileItem = new File(filename, false, fileSize, getDateTime());
                currDir->addFile(newFileItem);
                cout << "File '" << filename << "' created successfully." << endl;
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

            bool found = false;

            for (const auto& item : currDir->getItems()) {
                if (item->getName() == name) {
                    vector<systemItem*> removeItem{ item };
                    currDir->removeItem(removeItem);
                    delete item;
                    found = true;
                    break;
                }
            }
            if (found) {
                cout << "Deleted '" << name << "' successfully." << endl;
            }
            else {
                cout << "File or directory '" << name << "' not found." << endl;
            }
        }
        else {
            cout << "Invalid command: del requires a file or directory name." << endl;
        }
    }

    else if (cmd == "save") {
        ofstream file("store.txt");
        if (file.is_open()) {
            // Save commands
            for (const auto& item : constant->loadCommand()) {
                file << item << endl;
            }
            cout << "Saved to 'store.txt'" << endl;
            file.close();
        }
        else {
            cout << "Error: Can't find file" << endl;
        }
    }

    else if (cmd == "load") {
        ifstream file("store.txt");
        if (file.is_open()) {
            string line;
            while (getline(file, line) && line.length() > 0 && line != "dir" && line != "help") {
                executeCommand(line, items, path, currDir, constant);
            }
            cout << "Restored from 'store.txt'" << endl;
            file.close();
        }
        else {
            cout << "Error: Can't find file" << endl;
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

        initializeItems(path, currDir);
    }

    constant->storeCommand(command);


}