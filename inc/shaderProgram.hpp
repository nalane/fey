#pragma once

/*
 * Class contains and loads the shader program
 */

#include <vector>

#include "raw_resource.hpp"
#include "shader.hpp"

const std::string SHADER_KEY = "___MAIN_SHADER_KEY___";

class shaderProgram : public raw_resource {
private:
  std::vector<shader*> shaders;
  GLuint progID;

public:
  shaderProgram(std::string name = SHADER_KEY) : progID(-1), raw_resource(name) { }
  ~shaderProgram();
  
  void addShader(shader* s);
  bool loadShaders();
  bool compileShaders();
  bool linkShaders();
  void useProgram();
  GLuint getProgID() {return progID;}
};
