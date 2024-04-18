#include "Directories.h"

Directory::Directory(const std::string& name, bool isDir, const std::string& size, const std::string& date) : name(name), isDir(isDir), size(size), date(date) {}

std::string Directory::getName() const {
    return name;
}

bool Directory::isDirectory() const {
    return isDir;
}

std::string Directory::getSize() const {
    return size;
}

std::string Directory::getDate() const {
    return date;
}
