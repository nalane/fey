#pragma once

/*
 * A dummy class to test drawing a generic object
 */
 
#include "model.hpp"
#include "glHeaders.hpp"

#include <vector>

class object {
private:
  model* m;
  glm::mat4 modelMatrix;
  GLuint textureLocation;
  
public:
  object(GLuint textureLocation);
  ~object();
			
  void draw();
  glm::mat4 getModelMatrix() {return modelMatrix;}
};
