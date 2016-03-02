#pragma once

class object;
#include "graphicsBackend.hpp"

class object {
private:
  GLuint vao;
  GLuint vbo[2];

  graphicsBackend* gfx;

public:
  object(graphicsBackend* gfx, float positions[3][4], float colors[3][4]);
  ~object();
			
  void draw();
};
