#pragma once

#include <stdbool.h>

#include "paths.h"

void initLogging(const path file);
bool recordLog(char* message, ...);
