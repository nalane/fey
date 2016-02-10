#include "shaders.h"

#include <stdlib.h>
#include <stdio.h>

#include "log.h"

typedef char* fileText;

fileText readFile(char* filename) {
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

void freeText(fileText t) {
  free(t);
}

GLuint compileShaders(char* vertexSource, char* fragmentSource) {
  GLint compiled = 0;
  
  fileText vertexText = readFile(vertexSource);
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, (const char**)&vertexText, NULL);
  glCompileShader(vertexShader);
  freeText(vertexText);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);
  if (!compiled) {
    GLint maxLength = 0;
    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
    
    char* message = malloc(maxLength * sizeof(char));
    glGetShaderInfoLog(vertexShader, maxLength, &maxLength, message);
    
    recordLog("Compiling vertex shader failed with the following message:\n%s", message);
    free(message);
  }

  fileText fragmentText = readFile(fragmentSource);
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, (const char**)&fragmentText, NULL);
  glCompileShader(fragmentShader);
  freeText(fragmentText);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);
  if (!compiled) {
    GLint maxLength = 0;
    glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
    
    char* message = malloc(maxLength * sizeof(char));
    glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, message);
    
    recordLog("Compiling fragment shader failed with the following message:\n%s", message);
    free(message);
  }

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
  
  return prog;
}

void deleteShader(GLuint progID) {
  glDeleteProgram(progID);
}
