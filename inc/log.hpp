#pragma once

/*
 * Contains functions for writing to the logfile
 */

#include <string>

void initLogging(const std::string& file, bool debugFlag);
bool recordLog(const std::string& message);
void endLogging();
