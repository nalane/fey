#pragma once

/*
 * Abstract collision shape class
 */

enum collisionShapeType {
  UNDEFINED,
  BOX,
  PLANE,
  SPHERE
};

class collisionShape {
private:
  collisionShapeType shapeType;

  virtual bool isCollide(collisionShape* second) = 0;
};
