#include "input.hpp"
#include <vector>

using namespace std;

vector<int> keyMap(GLFW_KEY_LAST + 1);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  keyMap[key] = action;
}

int key_state(int key) {
  return keyMap[key];
}
