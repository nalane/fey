#include "opengl.hpp"

opengl::~opengl() {

}

void opengl::glfwHints() {
    // Force use of OpenGL core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, numAASamples);
}

bool opengl::enableDepthBuffer() {
    return true;
}

void opengl::draw() {

}

void opengl::resizeCallback() {
    
}