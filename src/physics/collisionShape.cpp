#include "physics/collisionShape.hpp"
#include "physics/collisionOBB.hpp"
#include "physics/collisionAABB.hpp"
#include "physics/collisionSphere.hpp"
#include "physics/collisionPlane.hpp"
#include "log.hpp"

bool collisionShape::isColliding(collisionShape* second) {
  // If either shape is undefined, error out
  if ((shapeType | second->getShapeType()) & UNDEFINED) {
    recordLog("Error: Undefined shape type in collision test");
    return false;
  }
  
  switch (shapeType | second->getShapeType()) {
  // OBB-to-OBB collisions
  case OBB: {
    return false;
  }

  // OBB-to-AABB collisions
  case OBB | AABB: {
    return false;
  }

  // OBB-to-Plane collisions
  case OBB | PLANE: {
    return false;
  }

  // OBB-to-Sphere collisions
  case OBB | SPHERE: {
    return false;
  }
    
  // AABB-to-AABB collisions
  case AABB: {
    collisionAABB* lhs = (collisionAABB*)this;
    collisionAABB* rhs = (collisionAABB*)second;

    glm::vec3 lhsMin = lhs->getMin();
    glm::vec3 lhsMax = lhs->getMax();
    glm::vec3 rhsMin = rhs->getMin();
    glm::vec3 rhsMax = rhs->getMax();

    // Detect overlap
    bool colliding = true;
    for (int i = 0; i < 3; i++)
      colliding = colliding && (lhsMin[i] <= rhsMax[i] && lhsMax[i] >= rhsMin[i]);
    return colliding;
  }

  // AABB-to-Plane collisions
  case AABB | PLANE: {
    collisionAABB* aabb = shapeType == AABB ? (collisionAABB*)this : (collisionAABB*)second;
    collisionPlane* plane = shapeType == PLANE ? (collisionPlane*)this : (collisionPlane*)second;

    double pLen = 0.0;
    for (int i = 0; i < 3; i++) {
      pLen += ((aabb->getHalfExtents())[i] * fabs((plane->getNormal())[i]));
    }
    double dist = glm::dot(plane->getNormal(), aabb->getCenter()) - plane->getDistance();

    return fabs(dist) < pLen;
  }

  // AABB-to-Sphere collisions
  case AABB | SPHERE: {
    collisionAABB* aabb = shapeType == AABB ? (collisionAABB*)this : (collisionAABB*)second;
    collisionSphere* sphere = shapeType == SPHERE ? (collisionSphere*)this : (collisionSphere*)second;

    // Find the point in the aabb closest to the sphere
    glm::vec3 aabbMin = aabb->getMin();
    glm::vec3 aabbMax = aabb->getMax();
    glm::vec3 closestPoint = sphere->getCenter();
    for (int i = 0; i < 3; i++) {
      if (closestPoint[i] > aabbMax[i])
	closestPoint[i] = aabbMax[i];
      else if (closestPoint[i] < aabbMin[i])
	closestPoint[i] = aabbMin[i];
    }

    return distance(closestPoint, sphere->getCenter()) <= sphere->getRadius();
  }

  // Plane-to-Plane collisions
  case PLANE: {
    collisionPlane* lhs = (collisionPlane*) this;
    collisionPlane* rhs = (collisionPlane*) second;

    return lhs->getNormal() != rhs->getNormal() || lhs->getDistance() == rhs->getDistance();
  }

  // Plane-to-Sphere collisions
  case PLANE | SPHERE: {
    collisionPlane* plane = shapeType == PLANE ? (collisionPlane*)this : (collisionPlane*)second;
    collisionSphere* sphere = shapeType == SPHERE ? (collisionSphere*)this : (collisionSphere*)second;

    double distanceFromPlane = glm::dot(plane->getNormal(), sphere->getCenter()) - plane->getDistance();
    return fabs(distanceFromPlane) <= sphere->getRadius();
  }
    
  // Sphere-to-Sphere collisions
  case SPHERE: {
    collisionSphere* lhs = (collisionSphere*)this;
    collisionSphere* rhs = (collisionSphere*)second;
    double radiusSum = lhs->getRadius() + rhs->getRadius();
    return distance(lhs->getCenter(), rhs->getCenter()) <= radiusSum;
  }
    
  // Unknown collisionshape type
  default: {
    recordLog("Error: Unknown collision shape type!");
    return false;
  }
  }
}
