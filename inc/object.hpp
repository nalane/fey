#pragma once

/*
 * A dummy class to test drawing a generic object
 */
 
#include "resource.hpp"
#include "model.hpp"
#include "glHeaders.hpp"

#include <vector>

class object : public resource {
private:
  model* m;
  GLuint textureLocation;
  
public:
  object(GLuint textureLocation);
  ~object();
			
  void load();
  void init();
  void update();
  void draw();
};
