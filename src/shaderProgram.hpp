#pragma once

/*
 * Class contains and loads the shader program
 */

#include <vector>

#include "shader.hpp"

class shaderProgram {
private:
  std::vector<shader*> shaders;
  GLuint progID;

public:
  shaderProgram() : progID(-1) {};
  ~shaderProgram();
  
  void addShader(shader* s);
  bool loadShaders();
  bool compileShaders();
  bool linkShaders();
  void useProgram();
};
