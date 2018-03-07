#pragma once

/*
 * Contains a material
 */

#include "glHeaders.hpp"
#include "alignedMemory.hpp"

#define ALIGNMENT 16

class alignas(ALIGNMENT) material {
private:
  glm::vec4 ambientVals;
  glm::vec4 diffuseVals;
  glm::vec4 specularVals;
  float specularIntensity;

public:
  material(const glm::vec4& ambientVals, const glm::vec4& diffuseVals,
	  const glm::vec4& specularVals, float specularIntensity) :
    ambientVals(ambientVals), diffuseVals(diffuseVals),
    specularVals(specularVals), specularIntensity(specularIntensity) { }

  material() : ambientVals(glm::vec4(0.0)), diffuseVals(glm::vec4(0.0)),
	       specularVals(glm::vec4(0.0)), specularIntensity(0.0f) { }

  void* operator new(size_t size) {
    return aligned_malloc(size, ALIGNMENT);
  }

  void operator delete(void* memory) {
    return aligned_free(memory);
  }
  
  glm::vec4 getAmbientVals() const { return ambientVals; }
  glm::vec4 getDiffuseVals() const { return diffuseVals; }
  glm::vec4 getSpecularVals() const { return specularVals; }
  float getSpecularIntensity() const { return specularIntensity; }
};
