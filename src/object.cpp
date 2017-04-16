#include "object.hpp"
#include "log.hpp"
#include "paths.hpp"

using namespace std;

// Removes "object" from GPU
object::~object() {

}

void object::addChild(object* child) {
  children.push_back(child);
  child->parent = this;
}

void object::removeChildren() {
  for (object* o : children) {
    delete o;
  }
}

void object::setShaderProg() {
  shaderProg = rHandler->loadShaderProg();
  texHandle = glGetUniformLocation(shaderProg.res->getProgID(), "texSampler");
}

glm::mat4 object::getModelMatrix() {
  if (parent == nullptr)
    return modelMatrix;
  else
    return parent->getModelMatrix() * modelMatrix;
}

void object::load() {
  this->setShaderProg();
  progID = shaderProg.res->getProgID();
  
  for (object* o : children)
    o->load();
}

void object::init() {
  for (object* o : children)
    o->init();
}

void object::update() {
  for (object* o : children)
    o->update();
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
  
  // Send model matrix to GPU
  GLint modelHandle = glGetUniformLocation(progID, "modelMatrix");
  glUniformMatrix4fv(modelHandle, 1, GL_FALSE, &modelMatrix[0][0]);
  
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
  
  mesh.res->bindTextureToUniform(texHandle);
  mesh.res->draw(progID);
  
  // Draw children
  for (object* o : children)
    o->draw();
}
