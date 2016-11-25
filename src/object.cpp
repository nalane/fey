#include "object.hpp"
#include "log.hpp"
#include "paths.hpp"

using namespace std;

// Removes "object" from GPU
object::~object() {

}

void object::load() {
  texHandle = glGetUniformLocation(rHandler->getShaderProg()->getProgID(), "texSampler");
}

void object::init() {

}

void object::update() {
  
}

// Tells GPU to render "object"
void object::draw() {
  glm::mat4 transformMatrix = rHandler->getActiveCamera()->getVPMatrix() * getModelMatrix();
  GLint mvpHandle = glGetUniformLocation(rHandler->getShaderProg()->getProgID(), "transformMatrix");
  glUniformMatrix4fv(mvpHandle, 1, GL_FALSE, &transformMatrix[0][0]);
}
