#include "skybox.hpp"
#include "vkSkybox.hpp"
#include "glSkybox.hpp"

using namespace std;

#define SIZE 333.3f

const vector<skyboxVertex> skybox::skyVertices = {
    // Positions
	{{-SIZE,  SIZE, -SIZE, 1.0f}},
    {{-SIZE, -SIZE, -SIZE, 1.0f}},
    {{SIZE, -SIZE, -SIZE, 1.0f}},
    {{SIZE, -SIZE, -SIZE, 1.0f}},
    {{SIZE,  SIZE, -SIZE, 1.0f}},
    {{-SIZE,  SIZE, -SIZE, 1.0f}},
  
    {{-SIZE, -SIZE,  SIZE, 1.0f}},
    {{-SIZE, -SIZE, -SIZE, 1.0f}},
    {{-SIZE,  SIZE, -SIZE, 1.0f}},
    {{-SIZE,  SIZE, -SIZE, 1.0f}},
    {{-SIZE,  SIZE,  SIZE, 1.0f}},
    {{-SIZE, -SIZE,  SIZE, 1.0f}},
  
    {{SIZE, -SIZE, -SIZE, 1.0f}},
    {{SIZE, -SIZE,  SIZE, 1.0f}},
    {{SIZE,  SIZE,  SIZE, 1.0f}},
    {{SIZE,  SIZE,  SIZE, 1.0f}},
    {{SIZE,  SIZE, -SIZE, 1.0f}},
    {{SIZE, -SIZE, -SIZE, 1.0f}},
   
    {{-SIZE, -SIZE,  SIZE, 1.0f}},
    {{-SIZE,  SIZE,  SIZE, 1.0f}},
    {{SIZE,  SIZE,  SIZE, 1.0f}},
    {{SIZE,  SIZE,  SIZE, 1.0f}},
    {{SIZE, -SIZE,  SIZE, 1.0f}},
    {{-SIZE, -SIZE,  SIZE, 1.0f}},
  
    {{-SIZE,  SIZE, -SIZE, 1.0f}},
    {{SIZE,  SIZE, -SIZE, 1.0f}},
    {{SIZE,  SIZE,  SIZE, 1.0f}},
    {{SIZE,  SIZE,  SIZE, 1.0f}},
    {{-SIZE,  SIZE,  SIZE, 1.0f}},
    {{-SIZE,  SIZE, -SIZE, 1.0f}},
  
    {{-SIZE, -SIZE, -SIZE, 1.0f}},
    {{-SIZE, -SIZE,  SIZE, 1.0f}},
    {{SIZE, -SIZE, -SIZE, 1.0f}},
    {{SIZE, -SIZE, -SIZE, 1.0f}},
    {{-SIZE, -SIZE,  SIZE, 1.0f}},
    {{SIZE, -SIZE,  SIZE, 1.0f}},
};

skybox* skybox::createSkybox(const std::string& name) {
    GraphicsLibrary lib = graphics::getInstance()->getLibrary();
    if (lib == VULKAN) {
        return new vkSkybox(name);
    }
    else if (lib == GL) {
        return new glSkybox(name);
    }

    return nullptr;
}