#include "shaderProgram.hpp"
#include "log.hpp"

using namespace std;

// Deletes shader program from GPU memory
shaderProgram::~shaderProgram() {
  glDeleteProgram(progID);
}

// Adds a shader to the program
void shaderProgram::addShader(shader* s) {
  shaders.push_back(s);
}

// Load all shaders into main memory
bool shaderProgram::loadShaders() {
  bool loaded = true;
  for (shader* s : shaders) {
    loaded &= s->load();
  }

  return loaded;
}

// Compile all shaders into GPU memory
bool shaderProgram::compileShaders() {
  bool compiled = true;
  for (shader* s : shaders) {
    compiled &= s->compile();
  }

  return compiled;
}

// Link shaders into a full program in GPU
bool shaderProgram::linkShaders() {
  progID = glCreateProgram();
  for (shader* s : shaders) {
    glAttachShader(progID, s->getID());
  }
  glLinkProgram(progID);

  GLint isLinked = 0;
  glGetProgramiv(progID, GL_LINK_STATUS, &isLinked);
  if (!isLinked) {
    GLint maxLength = 0;
    glGetProgramiv(progID, GL_INFO_LOG_LENGTH, &maxLength);
    
    char* message = new char[maxLength];
    glGetProgramInfoLog(progID, maxLength, &maxLength, message);
    
    recordLog("Linking shader program failed with the following message:\n" + string(message));
    delete[] message;
    
    return false;
  }
  else {
    recordLog("Successfully linked all shaders!");
    return true;
  }
}

// Tells GPU to use this program
void shaderProgram::useProgram() {
  glUseProgram(progID);
}
