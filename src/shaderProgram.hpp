#pragma once
#include <vector>
#include "shader.hpp"

class shaderProgram {
private:
  std::vector<shader*> shaders;
  GLuint progID;

public:
  shaderProgram();
  ~shaderProgram();
  void addShader(shader* s);
  bool loadShaders();
  bool compileShaders();
  bool linkShaders();
  GLuint getID() {return progID;};
};
