/*
 * Main reads the config file and initializes the game structures
 */

#include "paths.hpp"
#include "log.hpp"
#include "engine.hpp"
#include <iostream>

using namespace std;

// Takes a config file as optional argument;
// Otherwise, uses fey.cfg as the config file.
// Reads in the config file and initializes the graphics backend.
int main(int argc, char** argv) {
  const string DEFAULT_LOG_FILE = "/feyData/fey.log";
  const string DEFAULT_CONFIG_FILE = "/feyData/fey.cfg";

  // Find the path of the executable. Used to find resources.
  setUpProgramPath();
  string logPath = getFullPath(DEFAULT_LOG_FILE);
  string configPath = getFullPath(DEFAULT_CONFIG_FILE);

  // Enable debugging, if desired
  bool debug = false;
  for (int i = 1; i < argc; i++) {
    if (string(argv[i]) == "-d")
      debug = true;
    else
      configPath = argv[i];
  }
  initLogging(logPath, debug);
  
  // Start up the engine and run the game
  engineLoad(configPath);
  runGame();

  // Clean up and exit
  endEngine();
  endLogging();
  return EXIT_SUCCESS;
}
