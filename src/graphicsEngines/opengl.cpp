#include "opengl.hpp"
#include "scene.hpp"

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

bool opengl::initialize(bool fullscreen, unsigned int windowWidth, unsigned int windowHeight, const std::string& windowTitle, bool hideCursor, int numAASamples) {
    this->numAASamples = numAASamples;

    if (!initGLFW(fullscreen, windowWidth, windowHeight, windowTitle, hideCursor))
        return false;

    // Enable certain GL features
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    return true;
}

void opengl::draw() {
    // Clear the screen
    GLfloat color[] = {0.0, 0.0, 0.0, 1.0};
    glClearBufferfv(GL_COLOR, 0, color);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Draw the scene
    scene::getActiveScene()->draw();

    // Swap buffers
    glfwSwapBuffers(window);
}

void opengl::resizeCallback(int width, int height) {
    setDefaultAspectRatio((double) width / (double) height);
}