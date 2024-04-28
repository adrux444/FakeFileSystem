#pragma once
#pragma once

#include <string>
#include <vector>
#include "systemItem.h"
#include "Directories.h"

using namespace std;

void executeCommand(const string& command, vector<systemItem*>& items, string& path, Directory*& currDir, systemItem*& constant);

