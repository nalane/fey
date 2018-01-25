#include <vector>

#include "scene.hpp"

using namespace std;

// Standard key press procedure
void scene::keyPress(int key, int action, int mods) {
  pressedKeys[key] = (action == GLFW_PRESS || action == GLFW_REPEAT);
}

void scene::updateObjects() {
	vector<rigidBody*> rigidBodies;
    for (auto p : objects) {
		p.second->update();
		rigidBodies.push_back(p.second);
    }
	world.update(rigidBodies);
  }
