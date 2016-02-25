#pragma once

#include <stdbool.h>

#include "paths.h"

void initLogging(const path file, bool debugFlag);
bool recordLog(char* message, ...);
