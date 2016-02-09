#include "paths.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef __linux__
#include <unistd.h>
#elif _WIN32
#include <windows.h>
#endif

#define DIR_LENGTH 1024

path programPath = NULL;

// Gets the path the program is running from so that
// the data files can be easily read
void setUpProgramPath() {
  char* tmp = malloc(DIR_LENGTH * sizeof(char));
  
  #ifdef _WIN32
  GetCurrentDirectory(DIR_LENGTH, tmp);
  for (int i = strlen(tmp); i >= 0; i--) {
    if (tmp[i] == '\\') {
      tmp[i] = '/';
    }
  }
  #elif __linux__
  if (!readlink("/proc/self/exe", tmp, DIR_LENGTH))
    tmp = "\0";
  for (int i = strlen(tmp) - 1; i >= 0; i--) {
    if (tmp[i] == '/') {
      tmp[i] = '\0';
      break;
    }
  }
  #endif

  char* path = malloc((strlen(tmp) + 1) * sizeof(char));
  for (int i = 0; i < strlen(tmp); i++) {
    path[i] = tmp[i];
  }
  path[strlen(tmp)] = '\0';

  free(tmp);
  programPath = path;
}

// Convenient string concatenation
char* strConcatenate(char* s1, char* s2) {
  int size = strlen(s1) + strlen(s2) + 1;
  char* sum = malloc(size * sizeof(char));
  
  snprintf(sum, size, "%s%s", s1, s2);
  return sum;
}

// Get the full path for a file in the program's path
path getFullPath(path s) {
  return strConcatenate(programPath, s);
}

// Frees memory taken by a path
void freePath(path s) {
  free(s);
}

// Cleans up leftover memory
void cleanupPaths() {
  free(programPath);
}
