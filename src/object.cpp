#include "object.hpp"
#include "log.hpp"

using namespace std;

// Loads the "object" into GPU memory
object::object(vector<glm::vec3> positions, float colors[4][4]) {
  modelMatrix = glm::mat4(1.0);
  
  vector<int> indices;
  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(2);
  indices.push_back(3);
  m.setElementIndices(indices);
  
  m.setVertices(positions);
  m.addData(GL_ARRAY_BUFFER, 16 * sizeof(float), colors, GL_STATIC_DRAW, 1);
}

// Removes "object" from GPU
object::~object() {

}

// Tells GPU to render "object"
void object::draw() {
  m.draw();
}
