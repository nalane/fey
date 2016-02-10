/*
 * Contains functions for creating and using OpenGL shaders
 */

#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLuint compileShaders(char* vertexSource, char* fragmentSource);
void deleteShader(GLuint progID);
