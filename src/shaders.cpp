#include <fstream>
#include <vector>

#include "log.hpp"
#include "paths.hpp"
#include "shaders.hpp"

using namespace std;

// Reads an entire file into memory
string readFile(string filename) {
  string text = "";
  
  ifstream fin(filename.c_str());
  if (fin.is_open()) {
    string tmp;
    while (getline(fin, tmp)) {
      text += (tmp + "\n");
    }
    fin.close();
  }

  else {
    recordLog("Could not open the file " + filename);
  }
 
  return text;
}

// Loads a shader and returns its ID
// If an error occurred, uses shaderName to record message
GLuint loadShader(string sourceFile, GLenum shaderType, string shaderName) {
  string tmpText = readFile(sourceFile);
  GLuint shaderID = glCreateShader(shaderType);
  const char* data[] = {tmpText.c_str()};
  glShaderSource(shaderID, 1, data, NULL);
  glCompileShader(shaderID);

  GLint compiled = 0;
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
  if (!compiled) {
    GLint maxLength = 0;
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);
    
    char* message = new char[maxLength];
    glGetShaderInfoLog(shaderID, maxLength, &maxLength, message);
    
    recordLog("Compiling " + shaderName + " shader failed with the following message:\n" + message);
    delete[] message;
  }
  else {
    recordLog("Successfully compiled " + shaderName + " shader!");
  }
  
  return shaderID;
}

// Produces the id of the shader program
GLuint compileShaders(string vertexSource, string fragmentSource) {
  GLuint vertexShader = loadShader(vertexSource, GL_VERTEX_SHADER, "vertex");
  GLuint fragmentShader = loadShader(fragmentSource, GL_FRAGMENT_SHADER, "fragment");

  GLuint prog = glCreateProgram();
  glAttachShader(prog, vertexShader);
  glAttachShader(prog, fragmentShader);
  glLinkProgram(prog);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  GLint isLinked = 0;
  glGetProgramiv(prog, GL_LINK_STATUS, &isLinked);
  if (!isLinked) {
    GLint maxLength = 0;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &maxLength);
    
    char* message = new char[maxLength];
    glGetProgramInfoLog(prog, maxLength, &maxLength, message);
    
    recordLog("Linking shader program failed with the following message:\n" + string(message));
    delete[] message;
  }
  else {
    recordLog("Successfully linked all shaders!");
  }
  
  return prog;
}

void deleteShader(GLuint progID) {
  glDeleteProgram(progID);
}
