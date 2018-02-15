#include "collisionOBB.hpp"

using namespace std;

collisionOBB::collisionOBB(vector<glm::vec3> vertices) : collisionShape(OBB) {
  glm::vec3 min = vertices[0];
  glm::vec3 max = vertices[0];

  for (glm::vec3 v : vertices) {
    for (int i = 0; i < 3; i++) {
      if (v[i] < min[i])
        min[i] = v[i];

      if (v[i] > max[i])
        max[i] = v[i];
    }
  }
}

bool collisionOBB::pointInBox(glm::vec3 point) {
  return false;
}
