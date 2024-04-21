#include "Directories.h"
#include <string>

Directory::Directory(const string& name, bool isDir, const string& size, const string& date)
    : name(name), isDir(isDir), size(size), date(date) {}

void Directory::addSubDirectory(Directory* directory) {
    directory->parent = this;
    subDirectories.push_back(directory);
}

void Directory::addFile(File* file) {
    files.push_back(file);
}

const vector<Directory*>& Directory::getSubDirectories() const {
    return subDirectories;
}

const vector<systemItem*>& Directory::getFiles() const {
    return files;
}

vector<systemItem*> Directory::getItems() const {
    vector<systemItem*> items;
    items.reserve(subDirectories.size() + files.size());

    for (const auto& dir : subDirectories) {
        items.push_back(dir);
    }
    for (const auto& file : files) {
        items.push_back(file);
    }

    return items;
}

void Directory::setItems(const vector<systemItem*>& newItems) {
    items = newItems;
}

void Directory::removeItem(const vector<systemItem*>& itemsToRemove) {
    for (auto item : itemsToRemove) {
        auto it = find(items.begin(), items.end(), item);
        if (it != items.end()) {
            items.erase(it);
        }
    }
}