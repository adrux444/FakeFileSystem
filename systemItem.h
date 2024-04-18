#pragma once
#include <string>
#include <vector>

using namespace std;

class Directory;
class File;

class systemItem
{
public:
    virtual ~systemItem() {}
    virtual string getName() const = 0;
    virtual bool isDirectory() const = 0;
    virtual string getSize() const = 0;
    virtual string getDate() const = 0;
private:
protected:
};


class File : public systemItem
{
public:
    File(const string& name, bool isDir, const string& size, const string& date) : name(name), isDir(isDir), size(size), date(date)  {}
    string getName() const override { return name; }
    bool isDirectory() const override { return isDir; }
    string getSize() const override { return size; }
    string getDate() const override { return date; }
    
private:
    string name;
    bool isDir;
    string size;
    string date;
    
};

class Directory : public systemItem
{
public:
    Directory(const string& name, bool isDir, const string& size, const string& date) : name(name), isDir(isDir), size(size), date(date) {}
    string getName() const override { return name; }
    bool isDirectory() const override { return isDir; }
    string getSize() const override { return size; }
    string getDate() const override { return date; }

    void addSubDirectory(Directory* directory) {
        subDirectories.push_back(directory);
    }

    void addFile(File* file) {
        files.push_back(file);
    }

    vector<Directory*>& getSubDirectories()  {
        return subDirectories;
    }

    vector<File*>& getFiles()  {
        return files;
    }

private:
    string name;
    bool isDir;
    string size;
    string date;
    vector<Directory*> subDirectories;
    vector<File*> files; 
};
