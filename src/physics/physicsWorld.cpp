#include "physicsWorld.hpp"

using namespace std;

// Clear all forces and add all fundamentals
void physicsWorld::clearForces(const vector<rigidBody*>& rigidBodies) {    
  for (rigidBody* body : rigidBodies) {
    body->clearForce();
    if (body->hasFiniteMass()) {
      body->addForce(glm::vec3(0.0, GRAVITY_ACC * body->getMass(), 0.0));
    }
  }
}

// Add collision force to two objects that are colliding
void physicsWorld::collisionForces(rigidBody* a, rigidBody* b, double duration) {
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
		for (int j = 0; j < 3; j++) 
			if (impulse[j] < minForce[j])
				impulse[j] = minForce[j];
	
		  actors[i]->addForce(impulse);
    }
  }
}

// Update the physics of all objects in the world
void physicsWorld::update(const vector<rigidBody*>& rigidBodies) {
  // Update the time
  double newTime = glfwGetTime();
  double duration = newTime - time;
  time = newTime;

  clearForces(rigidBodies);

  // Compare each rigid body to all others to detect collisions
  for (int i = 0; i < rigidBodies.size() - 1; i++) {
	  rigidBody* a = rigidBodies[i];
	  for (int j = i + 1; j < rigidBodies.size(); j++) {
		  rigidBody* b = rigidBodies[j];
		  collisionForces(a, b, duration);
	  }
  }

  // Integrate motion of all bodies
  for (rigidBody* body : rigidBodies) {
    body->updatePhysics(duration);
  }
}