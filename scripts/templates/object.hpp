#pragma once

#include "object.hpp"

#include <string>

class @name@ : public object {
private:
  
public:
  @name@();
  ~@name@();
  
  void load();
  void init();
  void update();
  void draw();
};
