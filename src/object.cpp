#include "object.hpp"
#include "log.hpp"
#include "paths.hpp"
#include "feyModelLoader.hpp"

using namespace std;

// Loads the "object" into GPU memory
object::object(vector<glm::vec3> positions, float colors[4][4]) {
  modelMatrix = glm::mat4(1.0);
  
  vector<int> indices;
  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(2);
  indices.push_back(3);
  //  m.setElementIndices(indices);
  
  //m.setVertices(positions);
  //m.addData(GL_ARRAY_BUFFER, 16 * sizeof(float), colors, GL_STATIC_DRAW, 1);
  m = loadFeyModel(getFullPath("feyData/library/cube.fey.fey.model"));
}

// Removes "object" from GPU
object::~object() {
  delete m;
}

// Tells GPU to render "object"
void object::draw() {
  modelMatrix = modelMatrix * glm::rotate(glm::radians(0.2f), glm::vec3(0.0, 1.0, 0.0));
  m->draw();
}
