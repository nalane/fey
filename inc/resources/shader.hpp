#pragma once

/*
 * Class that stores a shader for the shader program
 */

#include "glHeaders.hpp"
#include "raw_resource.hpp"

#include <string>
#include <map>

class shader : public raw_resource {
private:
  GLuint shaderID;
  GLenum shaderType;
  std::string sourceFile;
  std::string source;
  std::map<GLuint, std::string> shaderDict;

public:
  shader(const std::string& sourceFile, GLenum shaderType);
  ~shader();

  bool load();
  bool compile();
  GLuint getID() const { return shaderID; }
};
