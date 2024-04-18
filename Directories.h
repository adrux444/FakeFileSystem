#pragma once
#include <string>
#include "systemItem.h"

class Directory :
    public systemItem
{
public:
    Directory(const std::string& name, bool isDir, const std::string& size, const std::string& date) : name(name), isDir(isDir), size(size), date(date) {}
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

