/*
 * This is the starting project for the Advanced Programming assessment - 2023-4
 * This project enumerates just the directory specified and does not descend into subdirectories
*/

#include <crtdbg.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <filesystem>  // file system needs cpp standard 17 or above.  This is enabled in this project, but if you recreated from scratch, set -std=c++17 flags
#include "systemItem.h"
#include "commands.h"
#include "directories.h"
#include "files.h"

using namespace std;

// function prototype of a helper method to convert timestamps
const tm convertTime(const filesystem::file_time_type& timestamp);

void initializeItems(const std::string& path, Directory* currentDir) {
    try {
        for (const auto& item : filesystem::directory_iterator(path))
        {
            if (item.is_directory()) // check if this is a directory
            {
                string name = item.path().filename().string();
                tm timestamp = convertTime(item.last_write_time());

                string dateTime = to_string(timestamp.tm_mday) + "/" + to_string(timestamp.tm_mon + 1) + "/" + to_string(timestamp.tm_year + 1900) + " " + to_string(timestamp.tm_hour) + ":" + to_string(timestamp.tm_min);

                Directory* dirItem = new Directory(name, true, " <DIR> ", dateTime);
                currentDir->addSubDirectory(dirItem);
            }
            else
            {
                string tmp = item.path().filename().string();
                const char* name = tmp.c_str();
                tm timestamp = convertTime(item.last_write_time());
                string dateTime = to_string(timestamp.tm_mday) + "/" + to_string(timestamp.tm_mon + 1) + "/" + to_string(timestamp.tm_year + 1900) + " " + to_string(timestamp.tm_hour) + ":" + to_string(timestamp.tm_min);

                __int64 filesize = item.file_size();
                string size = to_string(filesize);

                File* fileItem = new File(name, false, size, dateTime);
                currentDir->addFile(fileItem);
            }
        }
    }
    catch (const filesystem::filesystem_error& e)
    {
        cerr << "Error: " << e.what() << endl;
    }
}




// C++ entry point
int main()
{
#ifdef _DEBUG
    // make sure we are checking for memory leaks when the application ends, but only in the _DEBUG variant
    _onexit(_CrtDumpMemoryLeaks);
#endif

    // root path to enumerate
    string rootPath = ".";
    Directory* rootDir = new Directory(rootPath, true, " <DIR> ", "");
    cout << "Starting initialisation, please wait ...\n";

    initializeItems(rootPath, rootDir);
    cout << "Initialisation complete..." << endl;

    while (true) {
        std::string userInput;
        std::cout << rootPath << " :> ";
        std::getline(std::cin, userInput);

        std::vector<Directory*> items;

        executeCommand(userInput, items, rootPath);


        if (userInput == "exit") {
            break;
        }
    }
}


// we need to do a little conversion between what the filesystem returns for time stamps and something usable
// just use this function and don't worry about how it works
const tm convertTime(const filesystem::file_time_type& timestamp)
{
    using namespace chrono;  // just for this function, use chrono to save us prefixing chrono:: everywhere
    system_clock::time_point delta = time_point_cast<system_clock::duration>(timestamp - filesystem::file_time_type::clock::now() + system_clock::now());
    time_t conversion = system_clock::to_time_t(delta);

    tm result;
    localtime_s(&result, &conversion);
    return result;
}