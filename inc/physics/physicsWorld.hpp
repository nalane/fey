#pragma once

/*
 * Responsible for managing all physics in the world
 */
 
#include "glHeaders.hpp"
#include "rigidBody.hpp"

#include <vector>

class physicsWorld {
	private:
		static constexpr float GRAVITY_ACC = -3.0;
    
		double time;
    
	public:
		physicsWorld() : time(glfwGetTime()) { }

		void clearForces(const std::vector<rigidBody*>& rigidBodies);
		void collisionForces(rigidBody* a, rigidBody* b, double duration);
		void update(const std::vector<rigidBody*>& rigidBodies);
};