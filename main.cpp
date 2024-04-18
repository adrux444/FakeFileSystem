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
#include "commands.cpp"


using namespace std;
namespace fs = filesystem;

// function prototype of a helper method to convert timestamps
const tm convertTime(const fs::file_time_type& timestamp);

void initializeItems(const fs::path& rootPath, Directory& rootDirectory) {

    try {
        for (const auto& item : fs::directory_iterator(rootPath))
        {
            if (item.is_directory()) // check if this is a directory
            {
                string name = item.path().filename().string();
                tm timestamp = convertTime(item.last_write_time());
                string dateTime = to_string(timestamp.tm_mday) + "/" + to_string(timestamp.tm_mon + 1) + "/" + to_string(timestamp.tm_year + 1900) + " " + to_string(timestamp.tm_hour) + ":" + to_string(timestamp.tm_min);

                Directory* dirItem = new Directory(name, true, " <DIR> ", dateTime);
                rootDirectory.addSubDirectory(dirItem);
                initializeItems(item.path(), *dirItem);
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
                rootDirectory.addFile(fileItem);
            }
        }
    }
    catch (const fs::filesystem_error& e)
    {
        cerr << e.what() << '\n';
    }
}




// C++ entry point
int main() {
#ifdef _DEBUG
    // make sure we are checking for memory leaks when the application ends, but only in the _DEBUG variant
    _onexit(_CrtDumpMemoryLeaks);
#endif

    Directory rootDirectory(".", true, "", "");

    fs::path rootPath = "."; // or specify the root path you want to use

    vector<systemItem*> items;

    initializeItems(rootPath, rootDirectory);
    cout << "Initialization complete..." << endl;

    while (true) {
        string userInput;
        cout << rootPath << " :> ";
        getline(cin, userInput);

        string cmdPath = rootPath.generic_string();
        executeCommand(userInput, rootDirectory, cmdPath);


        if (userInput == "exit") {
            break;
        }
    }

    // Clean up allocated memory for items if needed
    for (auto& item : items) {
        delete item;
    }

    return 0;
}


// we need to do a little conversion between what the filesystem returns for time stamps and something usable
// just use this function and don't worry about how it works
const tm convertTime(const fs::file_time_type& timestamp)
{
    using namespace chrono;  // just for this function, use chrono to save us prefixing chrono:: everywhere
    system_clock::time_point delta = time_point_cast<system_clock::duration>(timestamp - fs::file_time_type::clock::now() + system_clock::now());
    time_t conversion = system_clock::to_time_t(delta);

    tm result;
    localtime_s(&result, &conversion);
    return result;
}