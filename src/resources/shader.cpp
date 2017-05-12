#include <fstream>
#include "shader.hpp"
#include "log.hpp"

using namespace std;

// Initializes class values and fills the dictionary
shader::shader(const string& sourceFile, GLenum shaderType) :
  sourceFile(sourceFile), shaderType(shaderType), shaderID(-1), raw_resource(sourceFile) {
  shaderDict[GL_VERTEX_SHADER] = "vertex";
  shaderDict[GL_GEOMETRY_SHADER] = "geometry";
  shaderDict[GL_FRAGMENT_SHADER] = "fragment";
  shaderDict[GL_COMPUTE_SHADER] = "compute";
}

// Removes shader from memory
shader::~shader() {
  glDeleteShader(shaderID);
}

// Reads an entire file into memory
bool shader::load() {
  source = "";
  
  ifstream fin(sourceFile.c_str());
  if (fin.is_open()) {
    string tmp;
    while (getline(fin, tmp)) {
      source += (tmp + "\n");
    }
    fin.close();
    return true;
  }

  else {
    recordLog("Could not open the file " + sourceFile);
    return false;
  }
}

// Compiles and produces an ID for the shader
bool shader::compile() {
  if (source.size() <= 0)
    return false;
  
  shaderID = glCreateShader(shaderType);
  const char* data[] = {source.c_str()};
  glShaderSource(shaderID, 1, data, nullptr);
  glCompileShader(shaderID);

  GLint compiled = 0;
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
  if (!compiled) {
    GLint maxLength = 0;
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);
    
    char* message = new char[maxLength];
    glGetShaderInfoLog(shaderID, maxLength, &maxLength, message);
    
    recordLog("Compiling " + shaderDict[shaderType] + " shader failed with the following message:\n" + message);
    delete[] message;
    return false;
  }
  else {
    recordLog("Successfully compiled " + shaderDict[shaderType] + " shader!");
    return true;
  }
}
