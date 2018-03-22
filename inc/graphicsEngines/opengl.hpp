#pragma once

/*
 * Rendering engine for OpenGL
 */

#include "graphics.hpp"
#include "glHeaders.hpp"

class opengl : public graphics {
private:
    int numAASamples;

    virtual void glfwHints();
    virtual bool initialize(bool fullscreen, unsigned int windowWidth, unsigned int windowHeight, const std::string& windowTitle, bool hideCursor, int numAASamples);

public:
    opengl() : graphics(GL) { }
    ~opengl();

    // Virtual methods from graphcis
    virtual void draw();
    virtual void resizeCallback(int width, int height);
};