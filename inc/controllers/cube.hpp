#pragma once

#include "object.hpp"
#include "resourceHandler.hpp"
#include "glHeaders.hpp"

#include <string>

class cube : public object {
private:
  
public:
  cube(resourceHandler* rHandler);
  ~cube();
  
  void load();
  void init();
  void update();
  void draw();
};
