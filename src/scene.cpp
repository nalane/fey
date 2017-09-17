#include <vector>

#include "scene.hpp"

using namespace std;

// Standard key press procedure
void scene::keyPress(int key, int action, int mods) {
  pressedKeys[key] = (action == GLFW_PRESS || action == GLFW_REPEAT);
}

// Clear all forces and add all fundamentals
void scene::physicsWorld::clearForces() {    
  for (auto p : owner->objects) {
    p.second->clearForce();
    if (p.second->hasFiniteMass()) {
      p.second->addForce(glm::vec3(0.0, GRAVITY_ACC * p.second->getMass(), 0.0));
    }
  }
}

// Add collision force to two objects that are colliding
void scene::physicsWorld::collisionForces(rigidBody* a, rigidBody* b, double duration) {
  if (a->isColliding(b)) {
    // Figure out which items have finite mass
    rigidBody* actors[2];
    int numActors = 0;
    if (a->hasFiniteMass()) 
      actors[numActors++] = a;
    if (b->hasFiniteMass()) 
      actors[numActors++] = b;

    // Calculate forces
    for (int i = 0; i < numActors; i++) {
      glm::vec3 diffP = glm::vec3(0.0, 0.0, 0.0) - actors[i]->getMomentum();
      glm::vec3 minForce = glm::vec3(0, -GRAVITY_ACC * actors[i]->getMass(), 0);
      glm::vec3 impulse = (float)(1 / duration) * diffP;
      for (int i = 0; i < 3; i++) 
	if (impulse[i] < minForce[i])
	  impulse[i] = minForce[i];
	  
      actors[i]->addForce(impulse);
    }
  }
}

// Update the physics of all objects in the world
void scene::physicsWorld::update() {
  // Update the time
  double newTime = glfwGetTime();
  double duration = newTime - time;
  time = newTime;

  clearForces();
  
  // Find all rigidbodies in scene
  vector<rigidBody*> rigidBodies;
  for (auto p : owner->objects)
    rigidBodies.push_back(p.second);

  // Compare each rigid body to all others to detect collisions
  while (rigidBodies.size() > 1) {
    rigidBody* b = rigidBodies.back();
    rigidBodies.pop_back();
    for (rigidBody* r : rigidBodies) {
      collisionForces(r, b, duration);
    }
  }

  // Integrate motion of all bodies
  for (auto p : owner->objects) {
    p.second->updatePhysics(duration);
  }
}
