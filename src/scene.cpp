#include <vector>

#include "scene.hpp"

using namespace std;

// Standard key press procedure
void scene::keyPress(int key, int action, int mods) {
  pressedKeys[key] = (action == GLFW_PRESS || action == GLFW_REPEAT);
}

// Update the physics of all objects in the world
void scene::physicsWorld::update() {
  // Update the time
  double newTime = glfwGetTime();
  double duration = newTime - time;
  time = newTime;
  
  // Clear all forces and add all fundamentals
  const float GRAVITY_ACC = -3.0;
  for (auto p : owner->objects) {
    p.second->clearForce();
    if (p.second->hasFiniteMass()) {
      p.second->addForce(glm::vec3(0.0, GRAVITY_ACC * p.second->getMass(), 0.0));
    }
  }

  // Firgure out collision forces
  vector<rigidBody*> rigidBodies;
  for (auto p : owner->objects)
    rigidBodies.push_back(p.second);

  while (rigidBodies.size() > 1) {
    rigidBody* b = rigidBodies.back();
    rigidBodies.pop_back();
    for (rigidBody* r : rigidBodies) {
      if (r->isColliding(b)) {
	// Figure out which items have finite mass
	rigidBody* actors[2];
	int numActors = 0;
	if (r->hasFiniteMass()) 
	  actors[numActors++] = r;
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
  }

  // Integrate motion of all bodies
  for (auto p : owner->objects) {
    p.second->updatePhysics(duration);
  }
}
