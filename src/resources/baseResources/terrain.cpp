#include "terrain.hpp"
#include "log.hpp"
#include "graphics.hpp"
#include "glTerrain.hpp"
#include "vkTerrain.hpp"

#include <fstream>
#include <cmath>

using namespace std;

terrain* terrain::createTerrain(const string& name) {
    GraphicsLibrary lib = graphics::getInstance()->getLibrary();
    if (lib == GL) {
        return new glTerrain(name);
    }

    if (lib == VULKAN) {
        return new vkTerrain(name);
    }

    return nullptr;
}