#pragma once
#include <string>
#include <vector>
#include <iostream>
class systemItem
{
public:
    virtual ~systemItem() {}
    virtual std::string getName() const = 0;
    virtual bool isDirectory() const = 0;
    virtual std::string getSize() const = 0;
    virtual std::string getDate() const = 0;
    void storeCommand(const std::string& command);
    std::vector<std::string> loadCommand() {
        return commandStore;
    }
    friend std::ostream& operator<<(std::ostream& string, const systemItem& item) {
        string << item.getDate() << "\t\t" << item.getSize() << "\t" << item.getName() << "\n";
        return string;
    }
private:
    std::vector<std::string> commandStore;
};
