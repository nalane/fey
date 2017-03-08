#include "second_scene.hpp"
#include "main_scene.hpp"

using namespace std;

second_scene::~second_scene() {
}

void second_scene::mousePosition(double xPos, double yPos) {
  if (loggedMouseLocation) {
    double diffX = xPos - mouseX;
    double diffY = yPos - mouseY;

    cam.res->pan(diffX / 25, -diffY / 25);
  }

  mouseX = xPos;
  mouseY = yPos;
  loggedMouseLocation = true;
}

void second_scene::load() {
  suz.load();
  c.load();
  
  suz.init();
  c.init();
  
  cam = rHandler->loadTrackBallCamera("tCam");
  cam.res->zoom(5.0);
  cam.res->setTarget(glm::vec3(0.0, 0.0, 0.0));
  cam.res->setUpVector(glm::vec3(0.0, 1.0, 0.0));
  rHandler->setActiveCamera("tCam");
  
  staticLight = rHandler->loadLight("static");
  staticLight.res->setPosition(glm::vec4(-1.0, 0.0, 0.0, 1.0));
  staticLight.res->setColor(glm::vec3(1.0, 1.0, 1.0));
}

bool second_scene::update() {
  if (pressedKeys[GLFW_KEY_SPACE])
    return true;

  if (pressedKeys[GLFW_KEY_UP])
    cam.res->zoom(-0.1);

  if (pressedKeys[GLFW_KEY_DOWN])
    cam.res->zoom(0.1);

  suz.update();
  
  return false;
}

void second_scene::draw() {
  c.draw();
  suz.draw();
}

scene* second_scene::nextScene() {
  return new main_scene(rHandler);
}
