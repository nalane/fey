#pragma once

/*
 * A gently rotating model of Suzanne for main_scene
 */

#include "object.hpp"

#include <string>

class monkey : public object {
private:
  
public:
  monkey();
  ~monkey();
  
  void load();
  void init();
  void update();
  void draw();
};
