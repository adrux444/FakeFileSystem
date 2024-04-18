#include "Files.h"

Files::Files(const std::string& name, bool isDir, const std::string& size, const std::string& date) : name(name), isDir(isDir), size(size), date(date) {}

std::string Files::getName() const {
    return name;
}

bool Files::isDirectory() const {
    return isDir;
}

std::string Files::getSize() const {
    return size;
}

std::string Files::getDate() const {
    return date;
}
