#include "terrain.hpp"
#include "log.hpp"
#include "graphics.hpp"
#include "glTerrain.hpp"

#include <fstream>
#include <cmath>

using namespace std;

terrain* terrain::createTerrain(const string& name) {
    GraphicsLibrary lib = graphics::getInstance()->getLibrary();
    if (lib == GL) {
        return new glTerrain(name);
    }

    return nullptr;
}

void terrain::loadTerrain() {
    // Open file
    ifstream fin(name.c_str());
    if (!fin) {
        recordLog("ERROR: Could not open file " + name);
        return;
    }

    // Each patch is a square; get the number of vertices on each side
    int patchSideLength = round(sqrt(PATCH_SIZE));

    // Get the width and height of the terrain
    int width, height;
    fin >> width >> height;
    if (width % patchSideLength != 0 || height % patchSideLength != 0) {
        recordLog("ERROR: Could not find a correct number of vertices in " + name);
    }

    // Load control points
    vector<vector<terrainVertex>> vertices;
    for (vector<terrainVertex>& v : vertices) {
        v.resize(width);
        for (terrainVertex& point : v)
            fin >> point.position.x >> point.position.y >> point.position.z;
    }

    // Close file
    fin.close();

    // Divide vertices into patches
    for (int i = 0; i < height; i += patchSideLength) {
        for (int j = 0; j < width; j += patchSideLength) {
            for (int k = 0; k < PATCH_SIZE; k++) {
                int rowNum = i + k / patchSideLength;
                int colNum = j + k % patchSideLength;
                controlPoints.push_back(vertices[rowNum][colNum]);
            }
        }
    }
}