#include "model.hpp"
#include "vkModel.hpp"
#include "log.hpp"

using namespace std;

// Helper function to create model based on what engine is in use
model* model::createModel(const std::string& name) {
    if (graphics::getInstance()->getLibrary() == VULKAN) {
        return new vkModel(name);
    }

    return nullptr;
}

// Adds a material
void model::addMaterial (const material& mat) {
  materials.push_back(mat);
}

// Creates a texture for the GPU
void model::setTexture(texture* tex) {
  child_resources["textures"][tex->getName()] = tex;
}

// Gather vertex data
void model::setVertices(const vector<glm::vec3>& vertexList) {
  vertices.resize(vertexList.size());
  for (int i = 0; i < vertexList.size(); i++) {
    vertices[i].position.x = vertexList[i].x;
    vertices[i].position.y = vertexList[i].y;
    vertices[i].position.z = vertexList[i].z;
    vertices[i].position.w = 1.0f;;
  }
  numVertices = vertexList.size();
}

// Finalize mapping of vertices to UV coordinates
void model::setUVMapping(const vector<glm::vec2>& uvList) {
  if (uvList.size() > 0) {
    if (numVertices != uvList.size()) {
      recordLog("ERROR: Number of vertices does not equal number of uv coordinates");
    }

    for (int i = 0; i < uvList.size(); i++) {
      vertices[i].vertexUV = uvList[i];
    }
  }
}

// Gather normals
void model::setNormals(const vector<glm::vec3>& normalList) {
  if (numVertices != normalList.size()) {
    recordLog("ERROR: Number of vertices does not equal number of normal vectors");
  }

  for (int i = 0; i < normalList.size(); i++) {
    vertices[i].normal.x = normalList[i].x;
    vertices[i].normal.y = normalList[i].y;
    vertices[i].normal.z = normalList[i].z;
    vertices[i].normal.w = 1.0f;
  }
}

vector<glm::vec3> model::getVertexPositions() const {
  vector<glm::vec3> positions;

  for (auto v : vertices) {
    positions.push_back(v.position);
  }

  return positions;
}