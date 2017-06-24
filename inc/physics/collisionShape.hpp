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

  virtual bool isCollide(collisionShape* second) = 0;

public:
  collisionShape(collisionShapeType shapeType) : shapeType(shapeType) { }
};
