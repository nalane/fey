#pragma once

/*
 * Base class for graphics engine classes
 */

#include "glHeaders.hpp"
#include <string>

enum GraphicsLibrary {
    GL = 0,
    VULKAN
};

class graphics {
protected:
    // Singleton
    static graphics* instance;

    GraphicsLibrary library;

    GLFWwindow* window;
    bool initGLFW(bool fullscreen, unsigned int windowWidth, unsigned int windowHeight, const std::string& windowTitle, bool hideCursor);
    virtual void glfwHints() = 0;
    virtual bool initialize(bool fullscreen, unsigned int windowWidth, unsigned int windowHeight, const std::string& windowTitle, bool hideCursor, int numAASamples) = 0;

public:
    graphics(GraphicsLibrary library) : library(library) { }
    virtual ~graphics();

    // Interact with singleton
    static bool createInstance(GraphicsLibrary library, bool fullscreen, unsigned int windowWidth, unsigned int windowHeight, const std::string& windowTitle, bool hideCursor, int numAASamples);
    static graphics* getInstance();
    static void endInstance();

    bool programShouldRun() {return !glfwWindowShouldClose(window);}

    GraphicsLibrary getLibrary() { return library; }

    // Turn on and turn off depth buffering
    virtual bool enableDepthBuffer() = 0;

    // Drawing method
    virtual void draw() = 0;

    // Commands to execute on window resize
    virtual void resizeCallback() = 0;
};