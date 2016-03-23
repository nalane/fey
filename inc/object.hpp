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
  object(std::vector<glm::vec3>, float colors[4][4], GLuint textureLocation);
  ~object();
			
  void draw();
  glm::mat4 getModelMatrix() {return modelMatrix;}
};
