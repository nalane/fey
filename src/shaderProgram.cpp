#include "shaderProgram.hpp"
#include "log.hpp"

using namespace std;

shaderProgram::shaderProgram() : progID(-1) {

}

shaderProgram::~shaderProgram() {
  glDeleteProgram(progID);
}

void shaderProgram::addShader(shader* s) {
  shaders.push_back(s);
}

bool shaderProgram::loadShaders() {
  bool loaded = true;
  for (shader* s : shaders) {
    loaded &= s->load();
  }

  return loaded;
}

bool shaderProgram::compileShaders() {
  bool compiled = true;
  for (shader* s : shaders) {
    compiled &= s->compile();
  }

  return compiled;
}

bool shaderProgram::linkShaders() {
  progID = glCreateProgram();
  for (shader* s : shaders) {
    glAttachShader(progID, s->getID());
  }
  glLinkProgram(progID);

  for (shader* s : shaders) {
    delete s;
    s = NULL;
  }

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
