#pragma once

#include "glHeaders.hpp"
#include "resourceHandler.hpp"

class bulletDebugDrawer : public btIDebugDraw {
 private:
  GLuint prog;
  GLuint vao;
  GLuint vbos[2];
  resourceHandler* rHandler;
  
 public:
  bulletDebugDrawer(resourceHandler* rHandler);
  virtual void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color);
  virtual void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color) { }
  virtual void reportErrorWarning(const char *warningString) { }
  virtual void draw3dText(const btVector3 &location, const char *textString) { }
  virtual void setDebugMode (int debugMode) { }
  virtual int getDebugMode() const { return 1; }
};
