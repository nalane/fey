/*
 * Contains functions for creating and using OpenGL shaders
 */

#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "paths.h"

GLuint compileShaders(path vertexSource, path fragmentSource);
void deleteShader(GLuint progID);
