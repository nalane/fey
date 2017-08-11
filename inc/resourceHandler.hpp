#pragma once

/*
 * Handles all data read in from a file and stores it in a resource
 */

#include <vector>
#include <string>

#define NO_RESOURCE_TEMPLATE // Prevent inclusion of template that requires full rh definition
#include "resource.hpp"
#undef NO_RESOURCE_TEMPLATE

#include "shader.hpp"
#include "shaderProgram.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "skybox.hpp"
#include "light.hpp"
#include "firstPersonCamera.hpp"
#include "trackBallCamera.hpp"

class resourceHandler {
private:
  // Singleton
  static resourceHandler* instance;

  // Default shaders
  std::string defaultVertexShader;
  std::string defaultFragmentShader;

  // Resource maps
  std::map<std::string, raw_resource*> resources;
  std::map<std::string, camera*> cameras;
  std::map<std::string, light*> lights;

  // Active camera
  std::string activeCameraID;

  model* loadFeyModel(const std::string& filename);
  shader* loadVertexShader(const std::string& vertexShaderPath);
  shader* loadFragmentShader(const std::string& fragmentShaderPath);
  shaderProgram* newShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& key);
  std::string getShaderKey(const std::string& vert, const std::string& frag);
  
public:
  ~resourceHandler();

  // Methods to interact with singleton
  static void createInstance();
  static resourceHandler* getInstance();
  static void endInstance();

  // Unload resources
  void unload(const std::string& name);
  void unloadAll();

  // Shader interaction
  void setDefaultShaderProg(const std::string& vertexShader, const std::string& fragmentShader);
  resource<shaderProgram> loadShaderProg(const std::string& vertexShader, const std::string& fragmentShader);
  resource<shaderProgram> loadShaderProg();

  // Miscellaneous
  resource<model> loadModel(const std::string& filepath);
  resource<skybox> loadSkybox(const std::string& path, const std::string& extension);

  // Interact with lights
  resource<light> loadLight(const std::string& lightName);
  std::vector<light*> getAllLights();

  // Cameras
  resource<camera> loadCamera(const std::string& name);
  resource<firstPersonCamera> loadFirstPersonCamera(const std::string& name);
  resource<trackBallCamera> loadTrackBallCamera(const std::string& name);

  void setActiveCamera(const std::string& id);
  camera* getActiveCamera();
};

#include "resource.tpp"
