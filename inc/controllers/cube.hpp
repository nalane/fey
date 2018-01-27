#pragma once

/*
 * A textured cube for second_scene
 */

#include "object.hpp"

#include <string>

class cube : public object {
private:
  
public:
  cube();
  ~cube();
  
  void load();
  void init();
  void update();
  void draw();
};
