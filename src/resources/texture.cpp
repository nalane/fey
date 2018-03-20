#include "texture.hpp"
#include "vkTexture.hpp"

using namespace std;

texture* texture::createTexture(const string& name, const set<string>& files) {
    if (graphics::getInstance()->getLibrary() == VULKAN) {
        return new vkTexture(name, files);
    }

    return nullptr;
}