#pragma once

/*
 * Contains functions for initializing and running the game.
 */

#include "graphicsBackend.h"

// Struct that stores the state of the game
typedef struct {
  int windowWidth;
  int windowHeight;
  char* windowTitle;
  graphicsBackend* gfx;
} game;

// Generates a bare game data structure from the config file
game* readConfig(const char* filename);

// Run the game
void runGame(game* g);

// Frees the memory used by the game
void destroyGame(game* g);
