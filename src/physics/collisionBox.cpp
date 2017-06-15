#include "physics/collisionBox.hpp"

using namespace std;

collisionBox::collisionBox(vector<glm::vec3> vertices) : collisionShape() {
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

  coordinates = translate(min) * scale(max - min);
}

bool collisionBox::pointInBox(glm::vec3 point) {
  glm::vec4 scaler = inverse(coordinates) * glm::vec4(point, 1.0f);
  return
    scaler.x >= 0.0f && scaler.x <= 1.0f &&
    scaler.y >= 0.0f && scaler.y <= 1.0f &&
    scaler.z >= 0.0f && scaler.z <= 1.0f;
}

bool collisionBox::isColliding(collisionShape* second) {
  return false;
}
