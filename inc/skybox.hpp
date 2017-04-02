#pragma once

/*
 * A skybox resource
 */

#include "raw_resource.hpp"
#include "shaderProgram.hpp"
#include "glHeaders.hpp"
#include "camera.hpp"

#include <string>

#define SKYBOX_RIGHT        0
#define SKYBOX_LEFT         1
#define SKYBOX_TOP          2
#define SKYBOX_BOTTOM       3
#define SKYBOX_BACK         4
#define SKYBOX_FRONT        5
#define NUM_SKYBOX_TEXTURES 6

class skybox : public raw_resource {
private:
  GLuint texID;
  GLuint vao;
  GLuint vbo;

  camera* activeCamera;
  
public:
  skybox(std::string name) : raw_resource(name), activeCamera(nullptr) { }
  ~skybox();

  void setActiveCamera(camera* cam) { this->activeCamera = cam; }
  void setShaderProgram(shaderProgram* shaderProg) {
    shaderProg->load();
    child_resources["shaderProg"] = shaderProg;
  }
  void setTextures(const char* texturePaths[NUM_SKYBOX_TEXTURES]);
  void draw();
};
