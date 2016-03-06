#include "object.hpp"
#include "log.hpp"

// Loads the "object" into GPU memory
object::object(graphicsBackend* gfx, float positions[3][4], float colors[3][4]) : gfx(gfx) {
  glGenVertexArrays(1, &vao);
  glGenBuffers(2, vbo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), positions, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), colors, GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
}

// Removes "object" from GPU
object::~object() {
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(2, vbo);
}

// Tells GPU to render "object"
void object::draw() {
  glDrawArrays(GL_TRIANGLES, 0, 3);
}
