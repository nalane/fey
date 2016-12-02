#pragma once

/*
 * Contains functions to get keyboard input
 */

#include "glHeaders.hpp"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
int key_state(int key);
