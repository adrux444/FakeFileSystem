#pragma once
#include <string>
#include "systemItem.h"

using namespace std;

class File :
    public systemItem
{
public:
    File(const string& name, bool isDir, const string& size, const string& date);
    std::string getName() const override { return name; }
    bool isDirectory() const override { return isDir; }
    std::string getSize() const override { return size; }
    std::string getDate() const override { return date; }
private:
    std::string name;
    bool isDir;
    std::string size;
    std::string date;
};
