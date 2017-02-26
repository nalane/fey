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
  glm::mat4 modelMatrix = getModelMatrix();
  GLint modelHandle = glGetUniformLocation(progID, "modelMatrix");
  glUniformMatrix4fv(modelHandle, 1, GL_FALSE, &modelMatrix[0][0]);
  
  glm::mat4 viewMatrix = rHandler->getActiveCamera()->getViewMatrix();
  GLint viewHandle = glGetUniformLocation(progID, "viewMatrix");
  glUniformMatrix4fv(viewHandle, 1, GL_FALSE, &viewMatrix[0][0]);
  
  glm::mat4 projectionMatrix = rHandler->getActiveCamera()->getProjectionMatrix();
  GLint projectionHandle = glGetUniformLocation(progID, "projectionMatrix");
  glUniformMatrix4fv(projectionHandle, 1, GL_FALSE, &projectionMatrix[0][0]);

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
