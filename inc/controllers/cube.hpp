#pragma once

#include "object.hpp"
#include "model.hpp"
#include "resourceHandler.hpp"
#include "shaderProgram.hpp"
#include "glHeaders.hpp"
#include "resource.hpp"

#include <string>

class cube : public object {
private:
  resource<model> m;
  
public:
  cube(resourceHandler* rHandler);
  ~cube();
  
  void load();
  void init();
  void update();
  void draw();
};
