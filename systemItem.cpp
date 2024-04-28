#include "systemItem.h"
#include <vector>
using namespace std;

void systemItem::storeCommand(const std::string& command) {
    commandStore.push_back(command);
}