#pragma once

/*
 * Considering the fact that paths given to fopen are relative to cwd,
 * we must give it the full path determined by the path of the program.
 */

#include <string>

void setUpProgramPath();
void setDataFolder(std::string relativeData);
void setLibraryFolder(std::string libraryInData);

std::string getFullPath(std::string s);
std::string getDataFolderPath(std::string s);
std::string getLibraryFolderPath(std::string s);
