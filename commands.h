#pragma once
#pragma once

#include <string>
#include <vector>
#include "systemItem.h"
#include "Directories.h"

using namespace std;

// Function prototypes
void executeCommand(const string& command, vector<systemItem*>& items, string& path, Directory*& currDir);

