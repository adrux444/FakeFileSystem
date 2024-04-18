#include "Directories.h"
#include <string>

// Constructor definition
Directory::Directory(const std::string& name, bool isDir, const std::string& size, const std::string& date) : name(name), isDir(isDir), size(size), date(date) {}
