#pragma once
#include <string>
#include <vector>
#include "systemItem.h"

using namespace std;

class Directory : public systemItem {
public:
    Directory(const string& name, bool isDir, const string& size, const string& date);
    string getName() const override { return name; }
    bool isDirectory() const override { return isDir; }
    string getSize() const override { return size; }
    string getDate() const override { return date; }
    void addSubDirectory(Directory* directory) {
        subDirectories.push_back(directory);
    }
    void addFile(systemItem* file) {
        files.push_back(file);
    }
    const vector<Directory*>& getSubDirectories() const {
        return subDirectories;
    }
    const vector<systemItem*>& getFiles() const {
        return files;
    }
private:
    string name;
    bool isDir;
    string size;
    string date;
    vector<Directory*> subDirectories;
    vector<systemItem*> files;
};
