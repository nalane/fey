/*
 * Main reads the config file and initializes the game structures
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "log.h"
#include "graphicsBackend.h"
#include "game.h"
#include "paths.h"

// Takes a config file as optional argument;
// Otherwise, uses opal.cfg as the config file.
// Reads in the config file and initializes the graphics backend.
int main(int argc, char** argv) {
  char DEFAULT_LOG_FILE[] = "/opalData/opal.log";
  char DEFAULT_CONFIG_FILE[] = "/opalData/opal.cfg";
  
  setUpProgramPath();
  path logPath = getFullPath(DEFAULT_LOG_FILE);
  path configPath = getFullPath(DEFAULT_CONFIG_FILE);

  bool debug = false;
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-d") == 0) {
      debug = true;
    }

    else {
      configPath = getFullPath(argv[i]);
    }
  }
  
  initLogging(logPath, debug);
  game* g = readConfig(configPath);

  if (!g)
    return EXIT_FAILURE;

  runGame(g);
  destroyGame(g);

  freePath(configPath);
  freePath(logPath);
  return EXIT_SUCCESS;
}
