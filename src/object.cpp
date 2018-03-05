#include "object.hpp"
#include "log.hpp"
#include "paths.hpp"
#include "scene.hpp"

using namespace std;

// Removes "object" from GPU
object::~object() {
  removeChildren();
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
  shaderProg = resourceHandler::getInstance()->loadShaderProg();
}

glm::mat4 object::getModelMatrix() const {
  if (parent == nullptr)
    return modelMatrix;
  else
    return parent->getModelMatrix() * modelMatrix;
}

void object::load() {
  this->setShaderProg();
  
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
  glm::mat4 viewMatrix = scene::getActiveScene()->getActiveCamera()->getViewMatrix();
  glm::mat4 projectionMatrix = scene::getActiveScene()->getActiveCamera()->getProjectionMatrix();
  
  // Get lights
  vector<light*> lights = scene::getActiveScene()->getAllLights();

  // Gather data for uniforms
  modelUniforms uniforms = {};
  uniforms.modelMatrix = modelMatrix;
  uniforms.viewMatrix = viewMatrix;
  uniforms.mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
  uniforms.mat = mesh.res->getMaterial();
  uniforms.numLights = lights.size();
  for (int i = 0; i < lights.size(); i++) {
    uniforms.lights[i] = *lights[0];
  }
  
  // Draw model
  mesh.res->draw(uniforms);
  
  // Draw children
  for (object* o : children)
    o->draw();
}

void object::translate(float x, float y, float z) {
  modelMatrix = glm::translate(glm::vec3(x, y, z)) * modelMatrix;
  if (collider != nullptr)
    collider->translate(x, y, z);
}

bool object::collidesWith(object* second) {
  if (collider == nullptr || second->collider == nullptr)
    return false;
  
  return collider->isColliding(second->collider);
}
