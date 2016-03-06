#pragma once

/*
 * Class that stores a shader for the shader program
 */

#include <GL/glew.h>
#define GLFW_NO_GLU 1
#define GLFW_INCLUDE_COREARB 1
#include <GLFW/glfw3.h>

#include <string>
#include <map>

class shader {
private:
  GLuint shaderID;
  GLenum shaderType;
  std::string sourceFile;
  std::string source;
  std::map<GLuint, std::string> shaderDict;

public:
  shader(std::string sourceFile, GLenum shaderType);
  ~shader();

  bool load();
  bool compile();
  GLuint getID() {return shaderID;} ;
};
