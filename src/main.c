#include <stdlib.h>
#include <stdio.h>

#include "log.h"
#include "graphicsBackend.h"

void drawingTest() {
  GLfloat color[] = {0.0, 0.0, 1.0, 1.0};
  glClearBufferfv(GL_COLOR, 0, color);
}

int main() {
  graphicsBackend* gfx = initGraphics();

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
