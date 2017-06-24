#pragma once

/*
 * Abstract collision shape class
 */

enum collisionShapeType {
  UNDEFINED,
  OBB,
  AABB,
  PLANE,
  SPHERE
};

class collisionShape {
private:
  collisionShapeType shapeType;

  virtual bool isColliding(collisionShape* second) = 0;

public:
  collisionShape(collisionShapeType shapeType) : shapeType(shapeType) { }
  collisionShapeType getShapeType() { return shapeType; }
};
