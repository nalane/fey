/*
 * Contains functions for creating and using OpenGL shaders
 */

#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

#include "paths.hpp"

GLuint compileShaders(std::string vertexSource, std::string fragmentSource);
void deleteShader(GLuint progID);
