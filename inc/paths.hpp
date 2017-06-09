#pragma once

/*
 * Considering the fact that paths given to fopen are relative to cwd,
 * we must give it the full path determined by the path of the program.
 */

#include <string>

void setUpProgramPath();
void setDataFolder(const std::string& relativeData);
void setLibraryFolder(const std::string& libraryInData);

std::string getFullPath(const std::string& s);
std::string getDataFolderPath(const std::string& s);
std::string getLibraryFolderPath(const std::string& s);
