#pragma once

/*
 * Handles all data read in from a file and stores it in a resource
 */

#include <string>
#include <map>

class model;
class resourceHandler;

#include "glHeaders.hpp"
#include "shader.hpp"
#include "shaderProgram.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "resource.hpp"

class resourceHandler {
private:
  std::map<std::string, resource*> resources;
  std::map<std::string, camera*> cameras;

  std::string activeCameraID;

  model* loadFeyModel(std::string filename);
  shader* loadVertexShader(std::string vertexShaderPath);
  shader* loadFragmentShader(std::string fragmentShaderPath);

public:
  resourceHandler() {};
  ~resourceHandler();

  model* loadModel(std::string filepath);
  shaderProgram* loadShaderProg(std::string vertexShader, std::string fragmentShader);
  shaderProgram* getShaderProg();

  void unloadAll();

  void setCamera(std::string id, camera* cam);
  camera* getCamera(std::string id);

  void setActiveCamera(std::string id);
  camera* getActiveCamera();
};
