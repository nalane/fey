#include "skybox.hpp"
#include "vkSkybox.hpp"

skybox* skybox::createSkybox(const std::string& name) {
    if (graphics::getInstance()->getLibrary() == VULKAN) {
        return new vkSkybox(name);
    }

    return nullptr;
}