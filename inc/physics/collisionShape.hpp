#pragma once

/*
 * Abstract collision shape class
 */

enum collisionShapeType {
  UNDEFINED = 0x1,
  OBB = 0x2,
  AABB = 0x4,
  PLANE = 0x8,
  SPHERE = 0x10
};

class collisionShape {
private:
  collisionShapeType shapeType;

public:
  collisionShape(collisionShapeType shapeType) : shapeType(shapeType) { }
  
  collisionShapeType getShapeType() { return shapeType; }

  bool isColliding(collisionShape* second);
};
