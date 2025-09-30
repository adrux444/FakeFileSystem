#include <crtdbg.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <filesystem> 
#include "systemItem.h"
#include "commands.h"
#include "directories.h"
#include "files.h"
#include <sstream>

using namespace std;

// function prototype of a helper method to convert timestamps
const tm convertTime(const filesystem::file_time_type& timestamp);

void initializeItems(const string& path, Directory* currentDir) {
    try {
        for (const auto& item : filesystem::directory_iterator(path))
        {
            if (item.is_directory()) 
            {
                string name = item.path().filename().string();
                tm timestamp = convertTime(item.last_write_time());

                stringstream dateTime;
                dateTime << put_time(&timestamp, "%d/%m/%Y %H:%M");

                Directory* dirItem = new Directory(name, true, "<DIR>", dateTime.str());
                currentDir->addSubDirectory(dirItem);
            }
            else
            {
                string tmp = item.path().filename().string();
                const char* name = tmp.c_str();
                tm timestamp = convertTime(item.last_write_time());
                stringstream dateTime;
                dateTime << put_time(&timestamp, "%d/%m/%Y %H:%M");

                __int64 filesize = item.file_size();
                string size = to_string(filesize);

                File* fileItem = new File(name, false, size, dateTime.str());
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
    _onexit(_CrtDumpMemoryLeaks);
#endif

    // root path to enumerate
    string rootPath = ".";
    Directory* rootDir = new Directory(rootPath, true, " <DIR> ", "");
    systemItem* constant = rootDir;
    cout << "Starting initialisation, please wait ...\n";

    initializeItems(rootPath, rootDir);
    cout << "Initialisation complete..." << endl;

    while (true) {
        std::string userInput;
        std::cout << rootPath << ":> ";
        std::getline(std::cin, userInput);

        std::vector<systemItem*> items;

        executeCommand(userInput, items, rootPath, rootDir, constant);


        if (userInput == "exit") {
            break;
        }
    }
}


const tm convertTime(const filesystem::file_time_type& timestamp)
{
    using namespace chrono;
    system_clock::time_point delta = time_point_cast<system_clock::duration>(timestamp - filesystem::file_time_type::clock::now() + system_clock::now());
    time_t conversion = system_clock::to_time_t(delta);

    tm result;
    localtime_s(&result, &conversion);
    return result;
}
