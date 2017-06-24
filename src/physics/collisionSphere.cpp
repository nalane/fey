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

bool collisionSphere::isColliding(collisionShape* second) {
  switch (second->getShapeType()) {
  // Sphere-to-Sphere collision
  case SPHERE: {
    collisionSphere* rhs = (collisionSphere*)second;
    double radiusSum = radius + rhs->radius;
    return distance(center, rhs->center) <= radiusSum;
  }
    
  // Unknown collisionshape type
  default: {
    recordLog("Error: Unknown collision shape type!");
    return false;
  }
  }
}
