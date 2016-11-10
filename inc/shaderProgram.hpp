#pragma once

/*
 * Class contains and loads the shader program
 */

#include <vector>

#include "resource.hpp"
#include "shader.hpp"

class shaderProgram : public resource {
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
  GLuint getProgID() {return progID;}
};
