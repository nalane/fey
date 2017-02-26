#pragma once

/*
 * Handles all data read in from a file and stores it in a resource
 */

#include <string>
#include <map>

template <typename T> class resource;
class model;
class resourceHandler;
class material;

#include "glHeaders.hpp"
#include "shader.hpp"
#include "shaderProgram.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "raw_resource.hpp"
#include "light.hpp"

class resourceHandler {
private:
  std::map<std::string, raw_resource*> resources;
  std::map<std::string, camera*> cameras;
  std::map<std::string, light*> lights;

  std::string activeCameraID;

  model* loadFeyModel(std::string filename);
  shader* loadVertexShader(std::string vertexShaderPath);
  shader* loadFragmentShader(std::string fragmentShaderPath);

public:
  resourceHandler() {};
  ~resourceHandler();

  resource<model> loadModel(std::string filepath);
  resource<shaderProgram> loadShaderProg(std::string vertexShader, std::string fragmentShader, bool defaultShader = false);
  resource<shaderProgram> loadShaderProg();

  void unload(std::string name);
  void unloadAll();

  void setCamera(std::string id, camera* cam);
  camera* getCamera(std::string id);

  void setActiveCamera(std::string id);
  camera* getActiveCamera();
};

#include "resource.hpp"
