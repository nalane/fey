#pragma once

/*
 * Rendering engine for OpenGL
 */

#include "graphics.hpp"

class opengl : public graphics {
private:
    int numAASamples;

    virtual void glfwHints();
    virtual bool initialize(bool fullscreen, unsigned int windowWidth, unsigned int windowHeight, const std::string& windowTitle, bool hideCursor, int numAASamples) {
        this->numAASamples = numAASamples;
        return initGLFW(fullscreen, windowWidth, windowHeight, windowTitle, hideCursor);
    }

public:
    opengl() : graphics(GL) { }
    ~opengl();

    // Virtual methods from graphcis
    virtual bool enableDepthBuffer();
    virtual void draw();
    virtual void resizeCallback();
};