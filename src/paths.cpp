#ifdef __linux__
  #include <unistd.h>
#elif _WIN32
  #include <windows.h>
#elif __APPLE__
  #include <mach-o/dyld.h>
#endif

#include <string>

#include "paths.hpp"

using namespace std;

string programPath = "";
string dataPath = "";
string libraryPath = "";

// Gets the path the program is running from so that
// the data files can be easily read
void setUpProgramPath() {
  const int DIR_LENGTH = 1024;
  char* tmp = new char[DIR_LENGTH];
  
#ifdef _WIN32
  GetModuleFileName(nullptr, tmp, DIR_LENGTH - 1);

  // Switch back slashes to forward slashes
  bool removedProgName = false;
  for (int i = strlen(tmp); i >= 0; i--) {
    if (tmp[i] == '\\') {
      if (!removedProgName) {
        tmp[i] = '\0';
        removedProgName = true;
      }
      else {
        tmp[i] = '/';
      }
    }
  }

#elif __linux__
  int numBytes = readlink("/proc/self/exe", tmp, DIR_LENGTH);
  if (numBytes < 0)
    numBytes = 0;
  
  // Remove the prog name from string
  for (int i = numBytes - 1; i >= 0; i--) {
    if (tmp[i] == '/') {
      tmp[i] = '\0';
      break;
    }
  }

#elif __APPLE__
  unsigned int size = DIR_LENGTH;
  _NSGetExecutablePath(tmp, &size);

  // Remove the prog name from string
  for (int i = strlen(tmp) - 1; i >= 0; i--) {
    if (tmp[i] == '/') {
      tmp[i] = '\0';
      break;
    }
  }
  
#endif

  programPath = tmp;
  delete[] tmp;
}

// As the config file is read in, we will set the path to the data folder
void setDataFolder(const string& relativeData) {
  dataPath = getFullPath(relativeData);
}

// Sets the library folder.
// Assumes it is in, or at least relative to, the data folder
void setLibraryFolder(const string& libraryInData) {
	libraryPath = getDataFolderPath(libraryInData);
}

// Get the full path for a file in the program's path
string getFullPath(const string& s) {
  return programPath + "/" + s;
}

// Given a file path in the data folder, finds full path
std::string getDataFolderPath(const string& s) {
	return dataPath + "/" + s;
}

// Given a file path in the library, finds full path
std::string getLibraryFolderPath(const string& s) {
	return libraryPath + "/" + s;
}
