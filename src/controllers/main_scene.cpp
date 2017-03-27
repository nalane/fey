#include "main_scene.hpp"
#include "second_scene.hpp"
#include <iostream>

using namespace std;

main_scene::~main_scene() {

}

void main_scene::mousePosition(double xPos, double yPos) {
  if (loggedMouseLocation) {
    double diffX = xPos - mouseX;
    double diffY = yPos - mouseY;
    
    cam.res->rotate(diffX / 250, glm::vec3(0.0, 1.0, 0.0));
    cam.res->rotate(diffY / 250, glm::vec3(1.0, 0.0, 0.0));
  }
  
  mouseX = xPos;
  mouseY = yPos;
  loggedMouseLocation = true;
}

void main_scene::load() {
  suz.load();
  suz.init();

  cam = rHandler->loadFirstPersonCamera("mainCam");
  cam.res->setPosition(glm::vec3(3.0, 3.0, 3.0));
  cam.res->setTarget(glm::vec3(0.0, 0.0, 0.0));
  cam.res->setUpVector(glm::vec3(0.0, 1.0, 0.0));
  rHandler->setActiveCamera("mainCam");
  
  mainLight = rHandler->loadLight("main");
  mainLight.res->setPosition(glm::vec4(1.5 * sin(rad), 0.0, 1.5 * cos(rad), 1.0));
  mainLight.res->setColor(glm::vec3(1.0, 1.0, 1.0));

  staticLight = rHandler->loadLight("static");
  staticLight.res->setPosition(glm::vec4(2.0, 2.0, 2.0, 1.0));
  staticLight.res->setColor(glm::vec3(0.7, 0.7, 0.7));
}

bool main_scene::update() {
  if (pressedKeys[GLFW_KEY_SPACE])
    return true;

  if (pressedKeys[GLFW_KEY_UP])
    cam.res->move(0.1, glm::vec3(0.0, 0.0, 1.0));

  if (pressedKeys[GLFW_KEY_DOWN])
    cam.res->move(0.1, glm::vec3(0.0, 0.0, -1.0));
  
  if (pressedKeys[GLFW_KEY_LEFT])
    cam.res->move(0.1, glm::vec3(-1.0, 0.0, 0.0));

  if (pressedKeys[GLFW_KEY_RIGHT])
    cam.res->move(0.1, glm::vec3(1.0, 0.0, 0.0));

  rad += 0.01;
  mainLight.res->setPosition(glm::vec4(1.5 * sin(rad), 2.0, 1.5 * cos(rad), 1.0));
  
  suz.update();
  
  return false;
}

void main_scene::draw() {
  suz.draw();
}

scene* main_scene::nextScene() {
  return new second_scene(rHandler);
}
