#pragma once

#include "object.hpp"

#include <string>

class ground : public object {
private:
  
public:
  ground();
  ~ground();
  
  void load();
  void init();
  void update();
  void draw();
};
