#include "object.hpp"
#include "log.hpp"

using namespace std;

// Loads the "object" into GPU memory
object::object(float positions[3][4], float colors[3][4]) {
	modelMatrix = glm::mat4(1.0);
  
  vector<int> indices;
  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(2);
  m.addData(GL_ARRAY_BUFFER, 12 * sizeof(float), positions, GL_STATIC_DRAW, 0);
  m.addData(GL_ARRAY_BUFFER, 12 * sizeof(float), colors, GL_STATIC_DRAW, 1);
  m.setElementIndices(indices);
}

// Removes "object" from GPU
object::~object() {

}

// Tells GPU to render "object"
void object::draw() {
	m.draw();
}
