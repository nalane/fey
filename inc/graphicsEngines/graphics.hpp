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

    GLFWwindow* window;
    bool initGLFW(bool fullscreen, unsigned int windowWidth, unsigned int windowHeight, const std::string& windowTitle, bool hideCursor);

    GraphicsLibrary library;

public:
    graphics(GraphicsLibrary library) : library(library) { }
    virtual ~graphics();

    virtual bool initialize(bool fullscreen, unsigned int windowWidth, unsigned int windowHeight, const std::string& windowTitle, bool hideCursor) = 0;

    // Interact with singleton
    static bool createInstance(GraphicsLibrary library, bool fullscreen, unsigned int windowWidth, unsigned int windowHeight, const std::string& windowTitle, bool hideCursor);
    static graphics* getInstance();
    static void endInstance();

    bool programShouldRun() {return !glfwWindowShouldClose(window);}

    GraphicsLibrary getLibrary() { return library; }

    // Turn on and turn off depth buffering
    virtual bool enableDepthBuffer() = 0;

    virtual void draw() = 0;

    virtual void resizeCallback() = 0;
};