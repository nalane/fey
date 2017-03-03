#include "object.hpp"
#include "log.hpp"
#include "paths.hpp"

using namespace std;

// Removes "object" from GPU
object::~object() {

}

void object::setShaderProg() {
  shaderProg = rHandler->loadShaderProg();
  texHandle = glGetUniformLocation(shaderProg.res->getProgID(), "texSampler");
}

void object::load() {
  this->setShaderProg();
  progID = shaderProg.res->getProgID();
}

void object::init() {

}

void object::update() {
  
}

// Tells GPU to render "object"
void object::draw() {
  // Get matrices
  glm::mat4 modelMatrix = getModelMatrix();
  glm::mat4 viewMatrix = rHandler->getActiveCamera()->getViewMatrix();
  glm::mat4 projectionMatrix = rHandler->getActiveCamera()->getProjectionMatrix();
  
  // Send view matrix to GPU
  GLint viewHandle = glGetUniformLocation(progID, "viewMatrix");
  glUniformMatrix4fv(viewHandle, 1, GL_FALSE, &viewMatrix[0][0]);
  
  // Send MV matrix to GPU
  glm::mat4 mvMatrix = viewMatrix * modelMatrix;
  GLint mvHandle = glGetUniformLocation(progID, "mvMatrix");
  glUniformMatrix4fv(mvHandle, 1, GL_FALSE, &mvMatrix[0][0]);

  
  // Send MVP matrix to GPU
  glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
  GLint mvpHandle = glGetUniformLocation(progID, "mvpMatrix");
  glUniformMatrix4fv(mvpHandle, 1, GL_FALSE, &mvpMatrix[0][0]);

  // Send lights to GPU
  vector<light*> lights = rHandler->getAllLights();
  GLint numLightsHandle = glGetUniformLocation(progID, "numLights");
  glUniform1i(numLightsHandle, lights.size());
  for (int i = 0; i < lights.size(); i++) {
    GLint lightHandle = glGetUniformLocation(progID, ("lights[" + to_string(i) + "].position").c_str());
    glUniform4fv(lightHandle, 1, lights[i]->getPosition());

    lightHandle = glGetUniformLocation(progID, ("lights[" + to_string(i) + "].color").c_str());
    glUniform3fv(lightHandle, 1, lights[i]->getColor());
  }
}
