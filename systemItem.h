#pragma once
#include <string>
class systemItem
{
public:
    virtual ~systemItem() {}
    virtual std::string getName() const = 0;
    virtual bool isDirectory() const = 0;
    virtual std::string getSize() const = 0;
    virtual std::string getDate() const = 0;
private:
protected:
};


class File : public systemItem
{
public:
    File(const std::string& name, bool isDir, const std::string& size, const std::string& date) : name(name), isDir(isDir), size(size), date(date) {}
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

class Directory : public systemItem
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
