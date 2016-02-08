/*
 * Main reads the config file and initializes the game structures
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "log.h"
#include "graphicsBackend.h"
#include "game.h"

// Takes a config file as optional argument;
// Otherwise, uses opal.cfg as the config file.
// Reads in the config file and initializes the graphics backend.
int main(int argc, char** argv) {
  const char DEFAULT_LOG_FILE[] = "opal.log";
  initLogging(DEFAULT_LOG_FILE);
  
  const char DEFAULT_CONFIG_FILE[] = "opal.cfg";
  game* g = NULL;
  if (argc == 2)
    g = readConfig(argv[1]);
  else
    g = readConfig(DEFAULT_CONFIG_FILE);

  if (!g)
    return EXIT_FAILURE;

  runGame(g);
  destroyGame(g);
}
