#pragma once

#include "light.hpp"
#include "material.hpp"

#define MAX_LIGHTS 5

struct modelUniforms {
  // Vertex shader uniforms
  glm::mat4 modelMatrix;
  glm::mat4 viewMatrix;
  glm::mat4 mvpMatrix;

  // Fragmaent shader uniforms
  material mat;
  glm::vec4 numLights;
  light lights[MAX_LIGHTS];
};