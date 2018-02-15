#include "second_scene.hpp"
#include "main_scene.hpp"
#include "paths.hpp"
#include "log.hpp"

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
  loadObjects();
  
  // Set up main light
  cam = resourceHandler::getInstance()->loadTrackBallCamera("tCam");
  cam.res->zoom(5.0);
  cam.res->setTarget(glm::vec3(0.0, 0.0, 0.0));
  cam.res->setUpVector(glm::vec3(0.0, 1.0, 0.0));
  resourceHandler::getInstance()->setActiveCamera("tCam");
  
  // Set up second light
  staticLight = resourceHandler::getInstance()->loadLight("static");
  staticLight.res->setPosition(glm::vec4(2.0, 2.0, 2.0, 1.0));
  staticLight.res->setColor(glm::vec3(1.0, 1.0, 1.0));

  // Set up sky box
  sky = resourceHandler::getInstance()->loadSkybox(getLibraryFolderPath("skybox/"), "tga");

  // Set up default shader
  defaultShader = resourceHandler::getInstance()->loadShaderProg();
}

bool second_scene::update() {
  if (pressedKeys[GLFW_KEY_SPACE])
    return true;

  if (pressedKeys[GLFW_KEY_UP])
    cam.res->zoom(-0.1);

  if (pressedKeys[GLFW_KEY_DOWN])
    cam.res->zoom(0.1);

  updateObjects();
  
  return false;
}

void second_scene::draw() {
  defaultShader.res->useProgram();
  drawObjects();
  sky.res->draw();
}

scene* second_scene::nextScene() {
  return new main_scene();
}
