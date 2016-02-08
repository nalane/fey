#pragma once

#include <stdbool.h>

void initLogging(const char* file);
bool recordLog(char* message, ...);
