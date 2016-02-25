#include "shaders.h"

#include <stdlib.h>
#include <stdio.h>

#include "log.h"
#include "paths.h"

typedef char* fileText;

// Reads an entire file into memory
fileText readFile(path filename) {
  FILE* f = fopen(filename, "rb");
  fileText text = NULL;

  if (f) {
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    text = malloc(size + 1);
    size = fread(text, 1, size, f);

    text[size] = '\0';
  }

  else {
    recordLog("Could not open the file %s\n", filename);
  }
  
  fclose(f);
  return text;
}

// Ensures the shaders are freed from memory
void freeText(fileText t) {
  free(t);
}

// Loads a shader and returns its ID
// If an error occurred, uses shaderName to record message
GLuint loadShader(path sourceFile, GLenum shaderType, char* shaderName) {
  fileText tmpText = readFile(sourceFile);
  GLuint shaderID = glCreateShader(shaderType);
  glShaderSource(shaderID, 1, (const char**)&tmpText, NULL);
  glCompileShader(shaderID);
  freeText(tmpText);

  GLint compiled = 0;
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
  if (!compiled) {
    GLint maxLength = 0;
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);
    
    char* message = malloc(maxLength * sizeof(char));
    glGetShaderInfoLog(shaderID, maxLength, &maxLength, message);
    
    recordLog("Compiling %s shader failed with the following message:\n%s", shaderName, message);
    free(message);
  }
  else {
    recordLog("Successfully compiled %s shader!", shaderName);
  }
  
  return shaderID;
}

// Produces the id of the shader program
GLuint compileShaders(path vertexSource, path fragmentSource) {
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
    
    char* message = malloc(maxLength * sizeof(char));
    glGetProgramInfoLog(prog, maxLength, &maxLength, message);
    
    recordLog("Linking shader program failed with the following message:\n%s", message);
    free(message);
  }
  else {
    recordLog("Successfully linked all shaders!");
  }
  
  return prog;
}

void deleteShader(GLuint progID) {
  glDeleteProgram(progID);
}
