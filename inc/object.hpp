#pragma once

/*
 * A dummy class to test drawing a generic object
 */
 
#include "model.hpp"

class object {
private:
  model m;
  glm::mat4 modelMatrix;

public:
  object(float positions[4][4], float colors[4][4]);
  ~object();
			
  void draw();
  glm::mat4 getModelMatrix() {return modelMatrix;}
};
