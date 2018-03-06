#include "second_scene.hpp"
#include "main_scene.hpp"
#include "paths.hpp"
#include "log.hpp"
#include "trackBallCamera.hpp"

using namespace std;

second_scene::~second_scene() {
}

void second_scene::mousePosition(double xPos, double yPos) {
  if (loggedMouseLocation) {
    double diffX = xPos - mouseX;
    double diffY = yPos - mouseY;

    trackBallCamera* cam = (trackBallCamera*)(cameras["cam"]);
    cam->pan(diffX / 25, -diffY / 25);
  }

  mouseX = xPos;
  mouseY = yPos;
  loggedMouseLocation = true;
}

void second_scene::load() {
  loadObjects();
  
  // Set up main camera
  trackBallCamera* cam = new trackBallCamera();
  cam->zoom(5.0);
  cam->setTarget(glm::vec3(0.0, 0.0, 0.0));
  cam->setUpVector(glm::vec3(0.0, 1.0, 0.0));
  cameras["cam"] = cam;
  setActiveCamera("cam");
  
  // Set up second light
  lights["staticLight"] = new light();
  lights["staticLight"]->setPosition(glm::vec4(2.0, 2.0, 2.0, 1.0));
  lights["staticLight"]->setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

  // Set up sky box
  sky = resourceHandler::getInstance()->loadSkybox(getLibraryFolderPath("skybox/"), "tga");

  // Set up default shader
  defaultShader = resourceHandler::getInstance()->loadShaderProg();
}

UpdateResult second_scene::update() {
  trackBallCamera* cam = (trackBallCamera*)(cameras["cam"]);

  if (pressedKeys[GLFW_KEY_SPACE])
    return CHANGE_SCENE;

  if (pressedKeys[GLFW_KEY_UP])
    cam->zoom(-0.1);

  if (pressedKeys[GLFW_KEY_DOWN])
    cam->zoom(0.1);

  updateObjects();
  
  return NO_CHANGE;
}

void second_scene::draw() {
  drawObjects();
  sky.res->draw();
}

scene* second_scene::nextScene() {
  return new main_scene();
}
