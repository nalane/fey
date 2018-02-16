#pragma once

/*
 * Class contains and loads the shader program
 */

#include <vector>

#include "raw_resource.hpp"
#include "shader.hpp"

class shaderProgram : public raw_resource {
private:
  GLuint progID;

public:
  shaderProgram(const std::string& name) : progID(-1), raw_resource(name) { }
  ~shaderProgram();
  
  void addShader(shader* s);
  bool loadShaders();
  bool compileShaders();
  bool linkShaders();
  void useProgram();
  GLuint getProgID() const { return progID; }
};
