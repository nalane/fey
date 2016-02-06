/*
 * Main reads the config file and initializes the game structures
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "log.h"
#include "graphicsBackend.h"

// Struct that stores the contents of the config file
typedef struct {
  int w;
  int h;
  char* windowTitle;
} configFile;

// Reads the config file and returns a struct storing its contents
configFile* readConfig(const char* filename) {
  configFile* cfg = malloc(sizeof(cfg));
  FILE* f = fopen(filename, "r");

  if (f) {
    fscanf(f, "%d\n", &cfg->w);
    fscanf(f, "%d\n", &cfg->h);

    char* title = malloc(128 * sizeof(char));
    fgets(title, 128, f);
    cfg->windowTitle = title;
    cfg->windowTitle[strlen(cfg->windowTitle) - 1] = '\0';
    
    fclose(f);
  }

  else {
    recordLog("FATAL ERROR: Could not read config file %s", filename);
  }
  
  return cfg;
}

// Primary drawing function. For now, just makes the screen blue
void drawingTest() {
  GLfloat color[] = {0.0, 0.0, 1.0, 1.0};
  glClearBufferfv(GL_COLOR, 0, color);
}

// Takes a config file as optional argument;
// Otherwise, uses opal.cfg as the config file.
// Reads in the config file and initializes the graphics backend.
int main(int argc, char** argv) {
  const char DEFAULT_FILE[] = "opal.cfg";
  configFile* cfg = NULL;
  if (argc == 2)
    cfg = readConfig(argv[1]);
  else
    cfg = readConfig(DEFAULT_FILE);

  if (!cfg)
    return EXIT_FAILURE;

  graphicsBackend* gfx = initGraphics(cfg->w, cfg->h, cfg->windowTitle);
  free(cfg);
  if (gfx) {
    drawToGraphics(gfx, drawingTest);
    terminateGraphics(gfx);
    return EXIT_SUCCESS;
  }

  else {
    fprintf(stderr, "A fatal error occurred. Check the log for more details.\n");
    return EXIT_FAILURE;
  }
}
