#ifdef __linux__
#include <unistd.h>
#elif _WIN32
#include <windows.h>
#endif

#include <string>

#include "paths.hpp"

using namespace std;

string programPath = "";

// Gets the path the program is running from so that
// the data files can be easily read
void setUpProgramPath() {
  const int DIR_LENGTH = 1024;
  char* tmp = new char[DIR_LENGTH];
  
  #ifdef _WIN32
  GetCurrentDirectory(DIR_LENGTH, tmp);
  for (int i = strlen(tmp); i >= 0; i--) {
    if (tmp[i] == '\\') {
      tmp[i] = '/';
    }
  }
  #elif __linux__
  int numBytes = readlink("/proc/self/exe", tmp, DIR_LENGTH);
  if (numBytes < 0)
    numBytes = 0;
  
  for (int i = numBytes - 1; i >= 0; i--) {
    if (tmp[i] == '/') {
      tmp[i] = '\0';
      break;
    }
  }
  #endif

  programPath = tmp;
  delete[] tmp;
}

// Get the full path for a file in the program's path
string getFullPath(string s) {
  return programPath + "/" + s;
}
