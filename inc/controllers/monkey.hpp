#pragma once

#include "object.hpp"
#include "model.hpp"
#include "resourceHandler.hpp"
#include "shaderProgram.hpp"
#include "glHeaders.hpp"
#include "resource.hpp"

#include <string>

class monkey : public object {
private:
  resource<model>* m;

public:
  monkey(resourceHandler* rHandler);
  ~monkey();
  
  void load();
  void init();
  void update();
  void draw();
};

#include "resource.cpp"
