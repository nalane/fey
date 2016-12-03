#pragma once

/*
 * A generic game object.
 */

#include "resourceHandler.hpp"
#include "shaderProgram.hpp"

#include <vector>
#include <string>

class object {
protected: 
  glm::mat4 modelMatrix;
  resourceHandler* rHandler;
  GLint texHandle;
  
public:
  object(resourceHandler* rHandler) : rHandler(rHandler) {};
  virtual ~object();
				
  virtual void load();
  virtual void init();
  virtual void update();
  virtual void draw();

  virtual glm::mat4 getModelMatrix() { return modelMatrix; };
};
