#pragma once

/*
 * Contains functions for writing to the logfile
 */

#include <string>

void initLogging(std::string file, bool debugFlag);
bool recordLog(std::string message);
