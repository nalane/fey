#include "game.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "log.h"
#include "paths.h"

// Primary drawing function. For now, just makes the screen blue
void drawingTest() {
  double currentTime = glfwGetTime();
  
  GLfloat color[] = {sin(currentTime) * 0.5 + 0.5,
		     cos(currentTime) * 0.5 + 0.5,
		     0.0, 1.0};
  glClearBufferfv(GL_COLOR, 0, color);
  glDrawArrays(GL_POINTS, 0, 1);
}

// Reads the config file and returns a struct storing its contents
game* readConfig(const path filename) {
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

    char* vertexShader = malloc(256 * sizeof(char));
    if (fgets(vertexShader, 256, f)) {
      g->vertexShader = vertexShader;
      if (strlen(vertexShader) > 0 &&
	  g->vertexShader[strlen(g->vertexShader) - 1] == '\n')
	g->vertexShader[strlen(g->vertexShader) - 1] = '\0';
      g->vertexShader = getFullPath(g->vertexShader);
      free(vertexShader);
    }
    else {
      recordLog("Could not read the vertex shader from the config file");
    }

    char* fragmentShader = malloc(256 * sizeof(char));
    if (fgets(fragmentShader, 256, f)) {
      g->fragmentShader = fragmentShader;
      if (strlen(fragmentShader) > 0 &&
	  g->fragmentShader[strlen(g->fragmentShader) - 1] == '\n')
	g->fragmentShader[strlen(g->fragmentShader) - 1] = '\0';
      g->fragmentShader = getFullPath(g->fragmentShader);
      free(fragmentShader);
    }
    else {
      recordLog("Could not read the fragment shader from the config file");
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
  g->gfx = initGraphics(g->windowWidth, g->windowHeight, g->windowTitle, g->vertexShader, g->fragmentShader);
  glGenVertexArrays(1, &(g->vao));
  glBindVertexArray(g->vao);
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
  glDeleteVertexArrays(1, &g->vao);
  terminateGraphics(g->gfx);
  free(g->windowTitle);
  freePath(g->vertexShader);
  freePath(g->fragmentShader);
  free(g);
}
