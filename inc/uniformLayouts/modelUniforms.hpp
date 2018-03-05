#pragma once

#include "light.hpp"
#include "material.hpp"

#define MAX_LIGHTS 50

struct modelUniforms {
  // Vertex shader uniforms
  glm::mat4 modelMatrix;
  glm::mat4 viewMatrix;
  glm::mat4 mvpMatrix;

  // Fragmaent shader uniforms
  material mat;
  int numLights;
  light lights[MAX_LIGHTS];
};