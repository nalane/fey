/*
 * Main reads the config file and initializes the game structures
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef __linux__
#include <unistd.h>
#elif _WIN32
#include <windows.h>
#endif

#include "log.h"
#include "graphicsBackend.h"
#include "game.h"

#define DIR_LENGTH 1024

// Gets the path the program is running from so that
// the data files can be easily read
char* getProgramPath() {
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
    return "";
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
  return path;
}

// Convenient string concatenation
char* strConcatenate(char* s1, char* s2) {
  int size = strlen(s1) + strlen(s2) + 1;
  char* sum = malloc(size * sizeof(char));
  
  snprintf(sum, size, "%s%s", s1, s2);
  return sum;
}

// Takes a config file as optional argument;
// Otherwise, uses opal.cfg as the config file.
// Reads in the config file and initializes the graphics backend.
int main(int argc, char** argv) {
  char DEFAULT_LOG_FILE[] = "/opalData/opal.log";
  char DEFAULT_CONFIG_FILE[] = "/opalData/opal.cfg";
  
  char* path = getProgramPath();
  char* logPath = strConcatenate(path, DEFAULT_LOG_FILE);
  char* configPath = strConcatenate(path, DEFAULT_CONFIG_FILE);
  
  initLogging(logPath);
  game* g = NULL;
  if (argc == 2)
    g = readConfig(argv[1]);
  else
    g = readConfig(configPath);

  if (!g)
    return EXIT_FAILURE;

  runGame(g);
  destroyGame(g);

  free(configPath);
  free(logPath);
  return EXIT_SUCCESS;
}
