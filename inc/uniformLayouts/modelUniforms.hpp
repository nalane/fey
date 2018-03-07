#pragma once

#include "light.hpp"
#include "material.hpp"

#define MAX_LIGHTS 5
#define ALIGNMENT 16

struct alignas(ALIGNMENT) modelUniforms {
  // Vertex shader uniforms
  glm::mat4 modelMatrix;
  glm::mat4 viewMatrix;
  glm::mat4 mvpMatrix;

  // Fragmaent shader uniforms
  material mat;
  int numLights;
  light lights[MAX_LIGHTS];
};