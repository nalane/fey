#include "main_scene.hpp"
#include "second_scene.hpp"
#include "paths.hpp"

using namespace std;

main_scene::~main_scene() {

}

void main_scene::mousePosition(double xPos, double yPos) {
  if (loggedMouseLocation) {
    double diffX = xPos - mouseX;
    double diffY = yPos - mouseY;

    firstPersonCamera* cam = (firstPersonCamera*)(cameras["mainCam"]);
    
    cam->rotate(diffX / 250, glm::vec3(0.0, 1.0, 0.0));
    cam->rotate(diffY / 250, glm::vec3(1.0, 0.0, 0.0));
  }
  
  mouseX = xPos;
  mouseY = yPos;
  loggedMouseLocation = true;
}

void main_scene::load() {
  loadObjects();

  // Set up camera
  cameras["mainCam"] = new firstPersonCamera();
  cameras["mainCam"]->setPosition(glm::vec3(3.0, 3.0, 3.0));
  cameras["mainCam"]->setTarget(glm::vec3(0.0, 0.0, 0.0));
  cameras["mainCam"]->setUpVector(glm::vec3(0.0, 1.0, 0.0));
  setActiveCamera("mainCam");
  
  // Set up main light
  lights["mainLight"] = new light();
  lights["mainLight"]->setPosition(glm::vec4(1.5 * sin(rad), 0.0, 1.5 * cos(rad), 1.0));
  lights["mainLight"]->setColor(glm::vec3(1.0, 1.0, 1.0));

  // Set up secondary light
  lights["staticLight"] = new light();
  lights["staticLight"]->setPosition(glm::vec4(2.0, 2.0, 2.0, 1.0));
  lights["staticLight"]->setColor(glm::vec3(0.7, 0.7, 0.7));
  
  // Set up sky box
  sky = resourceHandler::getInstance()->loadSkybox(getLibraryFolderPath("skybox_2/"), "tga");
  
  // Set up default shader
  defaultShader = resourceHandler::getInstance()->loadShaderProg();
}

bool main_scene::update() {
  firstPersonCamera* cam = (firstPersonCamera*)(cameras["mainCam"]);

  if (pressedKeys[GLFW_KEY_SPACE])
    return true;

  if (pressedKeys[GLFW_KEY_UP])
    cam->move(0.1, glm::vec3(0.0, 0.0, 1.0));

  if (pressedKeys[GLFW_KEY_DOWN])
    cam->move(0.1, glm::vec3(0.0, 0.0, -1.0));
  
  if (pressedKeys[GLFW_KEY_LEFT])
    cam->move(0.1, glm::vec3(-1.0, 0.0, 0.0));

  if (pressedKeys[GLFW_KEY_RIGHT])
    cam->move(0.1, glm::vec3(1.0, 0.0, 0.0));

  // Move main light
  rad += 0.01;
  lights["mainLight"]->setPosition(glm::vec4(1.5 * sin(rad), 2.0, 1.5 * cos(rad), 1.0));
  
  updateObjects();
  
  return false;
}

void main_scene::draw() {
  defaultShader.res->useProgram();
  drawObjects();
  
  sky.res->draw();
}

scene* main_scene::nextScene() {
  return new second_scene();
}
