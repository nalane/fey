#include "object.hpp"
#include "log.hpp"
#include "paths.hpp"

using namespace std;

// Removes "object" from memory
object::~object() {
  if (body != nullptr) {
    getWorld()->removeRigidBody(body);
    delete body;
  }
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
  shaderProg = rHandler->loadShaderProg();
  texHandle = glGetUniformLocation(shaderProg.res->getProgID(), "texSampler");
}

void object::addPhysicsRigidBody(string type) {
  btCollisionShape* shape;
  if (type == "hull") {
    btConvexHullShape* c = new btConvexHullShape();
    for (glm::vec3 vert : mesh.res->getVertices())
      c->addPoint(btVector3(vert.x, vert.y, vert.z));
    shape = c;
  }

  else if (type == "plane") {
    vector<glm::vec3> vertices = mesh.res->getVertices();
    glm::vec3 normal = cross(vertices[1] - vertices[0], vertices[2] - vertices[0]);
    shape = new btStaticPlaneShape(btVector3(normal.x, normal.y, normal.z), 0.0f);
  }
  
  btVector3 inertia;
  if (mass != 0.0f)
    shape->calculateLocalInertia(mass, inertia);

  btTransform objectTransformation;
  objectTransformation.setFromOpenGLMatrix(&modelMatrix[0][0]);
  modelMatrix = glm::mat4();
  btRigidBody::btRigidBodyConstructionInfo cInfo(mass, new btDefaultMotionState(objectTransformation),
						 shape, inertia);
  body = new btRigidBody(cInfo);
  getWorld()->addRigidBody(body);
  if (mass != 0.0) {
    //body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT); 
    //body->setActivationState(DISABLE_DEACTIVATION);
  }
}

glm::mat4 object::getModelMatrix() const {
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
  if (body != nullptr) {
    glm::mat4 physicsMatrix;
    btTransform transform;
    body->getMotionState()->getWorldTransform(transform);
    transform.getOpenGLMatrix(&physicsMatrix[0][0]);
    modelMatrix = physicsMatrix * modelMatrix;
  }
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
  //recordLog("MVP:");
  for (int i = 0; i < 3; i++) {
    string s;
    for (int j = 0; j < 3; j++) {
      s += (to_string(mvpMatrix[i][j]) + ", ");
    }
    //recordLog(s);
  }
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
