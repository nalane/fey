#include "@name@.hpp"

using namespace std;

@name@::~@name@() {
  unload();
}

void @name@::mousePosition(double xPos, double yPos) {
  mouseX = xPos;
  mouseY = yPos;
}

void @name@::load() {
  
}

bool @name@::update() {
  return false;
}

void @name@::draw() {
  
}

scene* @name@::unload() {
  
}
