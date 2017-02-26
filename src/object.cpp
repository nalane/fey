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
}
