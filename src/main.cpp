/*
 * Main reads the config file and initializes the game structures
 */

#include "paths.hpp"
#include "log.hpp"
#include "engine.hpp"
#include <iostream>

using namespace std;

// Takes a config file as optional argument;
// Otherwise, uses opal.cfg as the config file.
// Reads in the config file and initializes the graphics backend.
int main(int argc, char** argv) {
  const string DEFAULT_LOG_FILE = "/feyData/fey.log";
  const string DEFAULT_CONFIG_FILE = "/feyData/fey.cfg";

  setUpProgramPath();
  string logPath = getFullPath(DEFAULT_LOG_FILE);
  string configPath = getFullPath(DEFAULT_CONFIG_FILE);

  bool debug = false;
  for (int i = 1; i < argc; i++) {
    if (string(argv[i]) == "-d")
      debug = true;

    else
      configPath = argv[i];
  }

  initLogging(logPath, debug);
  
  engine* e = new engine(configPath);
  e->runGame();
  delete e;
  
  return EXIT_SUCCESS;
}
