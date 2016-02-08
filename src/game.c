#include "game.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "log.h"

// Primary drawing function. For now, just makes the screen blue
void drawingTest() {
  GLfloat color[] = {0.0, 0.0, 1.0, 1.0};
  glClearBufferfv(GL_COLOR, 0, color);
}

// Reads the config file and returns a struct storing its contents
game* readConfig(const char* filename) {
  game* g = malloc(sizeof(game));
  FILE* f = fopen(filename, "r");

  if (f) {
    if(!fscanf(f, "%d\n", &g->windowWidth))
      recordLog("Could not read the window width from the config file");
    if(!fscanf(f, "%d\n", &g->windowHeight))
      recordLog("Could not read the window height from the config file");

    char* title = malloc(128 * sizeof(char));
    if (fgets(title, 128, f)) {
      g->windowTitle = title;
      if (strlen(title) > 0 &&
	  g->windowTitle[strlen(g->windowTitle) - 1] == '\n')
	g->windowTitle[strlen(g->windowTitle) - 1] = '\0';
    }
    else {
      recordLog("Could not read the window title from the config file");
    }

    fclose(f);
  }

  else {
    recordLog("FATAL ERROR: Could not read config file %s", filename);
  }
  
  return g;
}

// Initializes the graphics system
bool initGame(game* g) {
  g->gfx = initGraphics(g->windowWidth, g->windowHeight, g->windowTitle);
  return (g->gfx != NULL);
}

// Run the game
void runGame(game* g) {
  if (initGame(g))
    drawToGraphics(g->gfx, drawingTest);

  else
    fprintf(stderr, "A fatal error occurred. Check the log for more details.\n");
}

// Free the game struct
void destroyGame(game* g) {
  terminateGraphics(g->gfx);
  free(g->windowTitle);
  free(g);
}
