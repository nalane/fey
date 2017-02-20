#pragma once

#include "object.hpp"
#include "model.hpp"
#include "resourceHandler.hpp"
#include "shaderProgram.hpp"
#include "glHeaders.hpp"
#include "resource.hpp"

#include <string>

class @name@ : public object {
private:
  resource<model> m;
  
public:
  @name@(resourceHandler* rHandler);
  ~@name@();
  
  void load();
  void init();
  void update();
  void draw();
};
