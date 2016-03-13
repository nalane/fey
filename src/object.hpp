#pragma once

/*
 * A dummy class to test drawing a generic object
 */
 
#include "glHeaders.hpp"

class object {
private:
  GLuint vao;
  GLuint vbo[3];
  
  glm::mat4 modelMatrix;

public:
  object(float positions[3][4], float colors[3][4]);
  ~object();
			
  void draw();
  glm::mat4 getModelMatrix() {return modelMatrix;}
};
