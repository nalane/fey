#include "graphics.hpp"
#include "scene.hpp"
#include "vulkan.hpp"

using namespace std;

// Singleton
graphics* graphics::instance;

// Static graphics callbacks
static void window_resize_callback(GLFWwindow* window, int width, int height) {
    graphics::getInstance()->resizeCallback();
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  scene::getActiveScene()->keyPress(key, action, mods);
}

static void cursor_callback(GLFWwindow* window, double xpos, double ypos) {
  scene::getActiveScene()->mousePosition(xpos, ypos);
}

graphics::~graphics() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

// Functions for interacting with singleton
void graphics::createInstance(GraphicsLibrary library, bool fullscreen, unsigned int windowWidth, unsigned int windowHeight, const string& windowTitle, bool hideCursor) {
  if (library == VULKAN) {
    instance = new vulkan();
    instance->initialize(fullscreen, windowWidth, windowHeight, windowTitle, hideCursor);
  }
}

graphics* graphics::getInstance() {
    return instance;
}

void graphics::endInstance() {
    delete instance;
}

// GLFW Initialization
bool graphics::initGLFW(bool fullscreen, unsigned int windowWidth, unsigned int windowHeight, const string& windowTitle, bool hideCursor) {
  if (!glfwInit()) {
    recordLog("FATAL ERROR: Could not initialize GLFW!");
    return false;
  }

  // Force use of OpenGL core
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  // Determine if we should do it fullscreen
  GLFWmonitor* monitor = nullptr;
  if (fullscreen) {
    monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
    windowWidth = videoMode->width;
    windowHeight = videoMode->height;
  }

  // Create the window
  window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), monitor, nullptr);
  if (!window) {
    recordLog("FATAL ERROR: Could not create a GLFW window!");
    glfwTerminate();
    return false;
  }

  // Set GLFW variables
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, cursor_callback);
  glfwSetWindowSizeCallback(window, window_resize_callback);
  if (hideCursor)
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  return true;
}