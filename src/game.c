#include "game.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "log.h"
#include "paths.h"

// Primary drawing function. For now, is experimental
void drawingTest() {
  double currentTime = glfwGetTime();
  
  GLfloat color[] = {0.0, 0.0, 0.0, 1.0};
  glClearBufferfv(GL_COLOR, 0, color);

  GLfloat attrib0[] = { (float)sin(currentTime) * 0.5f,
			(float)cos(currentTime) * 0.6f,
			0.0f, 0.0f};
  glVertexAttrib4fv(0, attrib0);

  GLfloat attrib1[] = { (float)rand() / RAND_MAX,
			(float)rand() / RAND_MAX,
			(float)rand() / RAND_MAX, 0.0f};
  glVertexAttrib4fv(1, attrib1);

  glDrawArrays(GL_TRIANGLES, 0, 3);
}

// Gets a string from a file and places it in dest
// In case of read error, uses varName to log error
void readStringFromConfig(FILE* f, char** dest, char* varName) {
  char* tmp = malloc(1024 * sizeof(char));
    if (fgets(tmp, 1024, f)) {
      if (strlen(tmp) > 0 && tmp[strlen(tmp) - 1] == '\n')
	tmp[strlen(tmp) - 1] = '\0';

      *dest = malloc((strlen(tmp) + 1) * sizeof(char));
      strcpy(*dest, tmp);
      free(tmp);
    }
    else {
      recordLog("Could not read the %s from the config file", varName);
    }
}

// Gets a path from a config file
void readPathFromConfig(FILE* f, path* dest, char* varName) {
  char* tmp = NULL;
  readStringFromConfig(f, &tmp, varName);
  *dest = getFullPath(tmp);
  free(tmp);
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

    readStringFromConfig(f, &g->windowTitle, "window title");
    readPathFromConfig(f, &g->vertexShader, "vertex shader");
    readPathFromConfig(f, &g->fragmentShader, "fragment shader");

    fclose(f);
  }

  else {
    recordLog("FATAL ERROR: Could not read config file %s", filename);
  }
  
  return g;
}

// Initializes the graphics system
bool initGame(game* g) {
  srand(time(NULL));
  
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
