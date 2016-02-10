#include "shaders.h"

#include <stdlib.h>
#include <stdio.h>

#include "log.h"

typedef char* fileText;

fileText readFile(char* filename) {
  FILE* f = fopen(filename, "r");
  
  fseek(f, 0, SEEK_END);
  int size = ftell(f);
  fseek(f, 0, SEEK_SET);

  fileText text = malloc(size + 1);
  size = fread(text, size, 1, f);
  fclose(f);

  text[size] = '\0';
  return text;
}

void freeText(fileText t) {
  free(t);
}

GLuint compileShaders(char* vertexSource, char* fragmentSource) {
  fileText vertexText = readFile(vertexSource);
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, (const char**)&vertexText, NULL);
  glCompileShader(vertexShader);
  freeText(vertexText);

  fileText fragmentText = readFile(fragmentSource);
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, (const char**)&fragmentText, NULL);
  glCompileShader(fragmentShader);
  freeText(fragmentText);

  GLuint prog = glCreateProgram();
  glAttachShader(prog, vertexShader);
  glAttachShader(prog, fragmentShader);
  glLinkProgram(prog);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  
  return prog;
}

void deleteShader(GLuint progID) {
  glDeleteProgram(progID);
}
