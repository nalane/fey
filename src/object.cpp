#include "object.hpp"
#include "log.hpp"

// Loads the "object" into GPU memory
object::object(graphicsBackend* gfx, float positions[3][4], float colors[3][4]) : gfx(gfx) {
  glGenVertexArrays(1, &vao);
  glGenBuffers(3, vbo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), positions, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), colors, GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
  
  GLuint indices[3] = {0, 1, 2};
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

// Removes "object" from GPU
object::~object() {
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(3, vbo);
}

// Tells GPU to render "object"
void object::draw() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);
}
