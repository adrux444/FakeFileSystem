#pragma once
#include <string>
#include <vector>
#include "systemItem.h"
#include "files.h"

using namespace std;

class Directory : public systemItem {
public:
    Directory(const string& name, bool isDir, const string& size, const string& date);
    string getName() const override { return name; }
    bool isDirectory() const override { return isDir; }
    string getSize() const override { return size; }
    string getDate() const override { return date; }
    void addSubDirectory(Directory* directory);
    void addFile(File* file);
    const vector<Directory*>& getSubDirectories() const;
    const vector<systemItem*>& getFiles() const;
    vector<systemItem*> getItems() const;
    void removeItem(const vector<systemItem*>& item);
    void setItems(const vector<systemItem*>& newItems);
    string getPath() const {
        if (parent == nullptr) {
            return name; // This is the root directory
        }
        else {
            return parent->getPath() + "\\" + name; // Concatenate parent's path with the directory name
        }
    }
    Directory* getParentDir() {
        return parent;
    }

private:
    string name;
    bool isDir;
    string size;
    string date;
    vector<Directory*> subDirectories;
    vector<systemItem*> files;
    vector<systemItem*> items;
    Directory* parent;
};
