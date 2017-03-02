#pragma once

/*
 * Contains a material
 */

class material;

#include "glHeaders.hpp"
#include "resourceHandler.hpp"

class material {
private:
  glm::vec4 ambientVals;
  glm::vec4 diffuseVals;
  glm::vec4 specularVals;
  float specularIntensity;

public:
  material(glm::vec4 ambientVals, glm::vec4 diffuseVals,
		   glm::vec4 specularVals, float specularIntensity) :
    ambientVals(ambientVals), diffuseVals(diffuseVals),
	specularVals(specularVals), specularIntensity(specularIntensity) { }
  
  glm::vec4 getAmbientVals() { return ambientVals; }
  glm::vec4 getDiffuseVals() { return diffuseVals; }
  glm::vec4 getSpecularVals() { return specularVals; }
  float getSpecularIntensity() { return specularIntensity; }

  void sendToShader(GLint progID);
};
