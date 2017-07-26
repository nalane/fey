#include "physics/collisionAABB.hpp"

using namespace std;

collisionAABB::collisionAABB(const vector<glm::vec3>& vertices) : collisionShape(AABB) {
  glm::vec3 min = vertices[0];
  glm::vec3 max = vertices[0];

  // Find min and max extents of the box
  for (int i = 1; i < vertices.size(); i++) {
    for (int j = 0; j < 3; j++) {
      if (vertices[i][j] > max[j])
	max[j] = vertices[i][j];
      if (vertices[i][j] < min[j])
	min[j] = vertices[i][j];
    }
  }

  // Calculate center and half extents
  origin = 0.5f * (min + max);
  halfExtents = 0.5f * (max - min);
}
