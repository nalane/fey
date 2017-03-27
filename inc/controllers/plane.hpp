#pragma once

#include "object.hpp"
#include "model.hpp"
#include "resourceHandler.hpp"
#include "shaderProgram.hpp"
#include "glHeaders.hpp"
#include "resource.hpp"

#include <string>

class plane : public object {
private:
  resource<model> m;
  
public:
  plane(resourceHandler* rHandler);
  ~plane();
  
  void load();
  void init();
  void update();
  void draw();
};
