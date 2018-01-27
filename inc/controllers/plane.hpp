#pragma once

/*
 * A generic plane for the second_scene
 */

#include "object.hpp"

#include <string>

class plane : public object {
private:
  
public:
  plane();
  ~plane();
  
  void load();
  void init();
  void update();
  void draw();
};
