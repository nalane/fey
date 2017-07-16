#include "physics/collisionSphere.hpp"
#include "log.hpp"

#include <cmath>

using namespace std;

collisionSphere::collisionSphere(vector<glm::vec3> points) : collisionShape(SPHERE) {
  // Average the points to find the center
  center = glm::vec3(0, 0, 0);
  for (glm::vec3 p : points) {
    center += p;
  }
  center /= points.size();

  // Max distance of points from center is radius
  double radiusSq = 0;
  for (glm::vec3 p : points) {
    glm::vec3 diff = p - center;
    radiusSq = dot(diff, diff);
  }
  radius = sqrt(radiusSq);
}
