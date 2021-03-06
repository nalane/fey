#pragma once

/*
 * Handles all data read in from a file and stores it in a resource
 */

#include <vector>
#include <string>
#include <map>

#define NO_RESOURCE_TEMPLATE // Prevent inclusion of template that requires full rh definition
#include "resource.hpp"
#undef NO_RESOURCE_TEMPLATE

#include "shaderProgram.hpp"
#include "model.hpp"
#include "skybox.hpp"
#include "texture.hpp"
#include "terrain.hpp"

class resourceHandler {
private:
  // Singleton
  static resourceHandler* instance;

  // Resource map
  std::map<std::string, raw_resource*> resources;
  std::map<std::string, shaderProgram*> shaders;

  model* loadFeyModel(const std::string& filename);
  template <typename T>
  shaderProgram* newShader(const std::map<std::string, std::string>& shaderList, bool depthEnable = true, bool cullModeBackFaces = true);
  std::string getShaderKey(const std::map<std::string, std::string>& shaderList) const;
  
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
  template <typename T>
  resource<shaderProgram> loadShaderProg(const std::map<std::string, std::string>& shaderList);
  void unloadShaders();
  void reloadShaders();

  // Miscellaneous
  resource<model> loadModel(const std::string& filepath);
  resource<texture> loadTexture(const std::set<std::string>& filepaths);
  resource<skybox> loadSkybox(const std::string& path, const std::string& extension);
  resource<terrain> loadTerrain(const std::string& path);
};

#include "resource.tpp"
