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
  shaderProgram* prog;
  GLint texHandle;

  std::string vertexShader;
  std::string fragmentShader;
  
public:
  object(resourceHandler* rHandler, std::string vertexShader, std::string fragmentShader) : 
    rHandler(rHandler), vertexShader(vertexShader), fragmentShader(fragmentShader) {};
  ~object();
				
  virtual void load();
  virtual void init();
  virtual void update();
  virtual void draw();

  virtual glm::mat4 getModelMatrix() { return modelMatrix; };
};
