#pragma once

/*
 * Contains a material
 */

#include "glHeaders.hpp"

class material {
private:
  glm::vec4 ambientVals;
  glm::vec4 diffuseVals;
  glm::vec4 specularVals;

public:
  material(glm::vec4 ambientVals, glm::vec4 diffuseVals, glm::vec4 specularVals) :
    ambientVals(ambientVals), diffuseVals(diffuseVals), specularVals(specularVals) { }
  
  glm::vec4 getAmbientVals() { return ambientVals; }
  glm::vec4 getDiffuseVals() { return diffuseVals; }
  glm::vec4 getSpecularVals() { return specularVals; }
};
