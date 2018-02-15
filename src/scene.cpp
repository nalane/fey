#include <vector>

#include "scene.hpp"

using namespace std;

scene* scene::activeScene;

scene::~scene() {
  for (auto p : objects) {
    delete p.second;
  }
  
  for (auto p : lights) {
    delete p.second;
  }

  for (auto p : cameras) {
    delete p.second;
  }
}

// Get singleton
scene* scene::getActiveScene() {
  return activeScene;
}

// Set the singleton
void scene::setActiveScene(scene* newScene) {
  activeScene = newScene;
}

// Delete active scene
void scene::endActiveScene() {
  delete activeScene;
  activeScene = nullptr;
}

// Standard key press procedure
void scene::keyPress(int key, int action, int mods) {
  pressedKeys[key] = (action == GLFW_PRESS || action == GLFW_REPEAT);
}

void scene::updateObjects() {
  // Get all rigidbodies in scene
	vector<rigidBody*> rigidBodies;
  for (auto p : objects) {
		p.second->update();
		rigidBodies.push_back(p.second);
  }

  // Update physics
	world.update(rigidBodies);
}

// Set ID of active camera
void scene::setActiveCamera(const std::string& id) {
  activeCameraID = id;
}

// Get the active cam
camera* scene::getActiveCamera() {
  return cameras[activeCameraID];
}

// Get all lights in the scene
std::vector<light*> scene::getAllLights() {
  vector<light*> lightList;
  for (auto p : lights) {
    lightList.push_back(p.second);
  }
  return lightList;
}
