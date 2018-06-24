#include "opengl.hpp"
#include "scene.hpp"

opengl::~opengl() {

}

void opengl::glfwHints() {
    // Force use of OpenGL core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, numAASamples);
}

void opengl::initialize(bool fullscreen, unsigned int windowWidth, unsigned int windowHeight, const std::string& windowTitle, bool hideCursor, int numAASamples) {
    this->numAASamples = numAASamples;

    initGLFW(fullscreen, windowWidth, windowHeight, windowTitle, hideCursor);

    // Activate OpenGL functions with GLAD
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        error("GLAD couldn't load extensions");
    }

    // Enable certain GL features
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
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