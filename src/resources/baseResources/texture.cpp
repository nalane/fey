#include "texture.hpp"
#include "vkTexture.hpp"
#include "glTexture.hpp"

using namespace std;

texture* texture::createTexture(const string& name, const set<string>& files) {
    GraphicsLibrary lib = graphics::getInstance()->getLibrary();
    if (lib == VULKAN) {
        return new vkTexture(name, files);
    }
    else if (lib == GL) {
        return new glTexture(name);
    }

    return nullptr;
}