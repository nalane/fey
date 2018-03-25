#include "graphics.hpp"
#include "scene.hpp"
#include "vulkan.hpp"
#include "opengl.hpp"

#include <exception>

using namespace std;

// Singleton
graphics* graphics::instance;

// Static graphics callbacks
static void window_resize_callback(GLFWwindow* window, int width, int height) {
    graphics::getInstance()->resizeCallback(width, height);
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
bool graphics::createInstance(GraphicsLibrary library, bool fullscreen, unsigned int windowWidth, unsigned int windowHeight, const string& windowTitle, bool hideCursor, int numAASamples) {
  if (library == VULKAN) {
    instance = new vulkan();
    try {
      instance->initialize(fullscreen, windowWidth, windowHeight, windowTitle, hideCursor, numAASamples);
      return true;
    } catch (exception& e) {
      delete instance;
      instance = nullptr;

      recordLog("WARNING: Could not initialize Vulkan: " + string(e.what()));
      recordLog("Attempting to initialize OpenGL engine instead...");
      library = GL;
    }
  }

  if (library == GL) {
    instance = new opengl();
    try {
      instance->initialize(fullscreen, windowWidth, windowHeight, windowTitle, hideCursor, numAASamples);
      return true;
    } catch (exception& e) {
      delete instance;
      instance = nullptr;

      recordLog("FATAL ERROR: Could not initialize OpenGL: " + string(e.what()));
      recordLog("No other graphics engines exist to test. Ending program.");
    }
  }

  return false;
}

graphics* graphics::getInstance() {
    return instance;
}

void graphics::endInstance() {
  if(instance != nullptr)
    delete instance;
}

// GLFW Initialization
bool graphics::initGLFW(bool fullscreen, unsigned int windowWidth, unsigned int windowHeight, const string& windowTitle, bool hideCursor) {
  if (!glfwInit()) {
    recordLog("FATAL ERROR: Could not initialize GLFW!");
    return false;
  }

  // Hints to see how to initialize GLFW context
  glfwHints();

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