#include "material.hpp"

void material::sendToShader(GLint progID) const {
  GLint ambientHandle = glGetUniformLocation(progID, "mat.ambient");
  glUniform4fv(ambientHandle, 1, &ambientVals[0]);

  GLint diffuseHandle = glGetUniformLocation(progID, "mat.diffuse");
  glUniform4fv(diffuseHandle, 1, &diffuseVals[0]);

  GLint specularHandle = glGetUniformLocation(progID, "mat.specular");
  glUniform4fv(specularHandle, 1, &specularVals[0]);
  
  GLint specularIntHandle = glGetUniformLocation(progID, "mat.specularIntensity");
  glUniform1f(specularIntHandle, specularIntensity);
}
