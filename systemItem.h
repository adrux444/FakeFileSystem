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

