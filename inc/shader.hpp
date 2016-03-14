#pragma once

/*
 * Class that stores a shader for the shader program
 */

#include "glHeaders.hpp"

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
