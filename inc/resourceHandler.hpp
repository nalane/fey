#pragma once

/*
 * Handles all data read in from a file and stores it in a resource
 */

#include <vector>
#include <string>

#define NO_RESOURCE_TEMPLATE // Prevent inclusion of template that requires full rh definition
#include "resource.hpp"
#undef NO_RESOURCE_TEMPLATE

#include "shaderProgram.hpp"
#include "model.hpp"
#include "skybox.hpp"
#include "texture.hpp"

class resourceHandler {
private:
  // Singleton
  static resourceHandler* instance;

  // Default shaders
  std::string defaultVertexShader;
  std::string defaultFragmentShader;

  // Resource map
  std::map<std::string, raw_resource*> resources;
  std::map<std::string, shaderProgram*> shaders;

  model* loadFeyModel(const std::string& filename);
  template <typename T>
  shaderProgram* newShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& key, VkBool32 depthEnable = VK_TRUE, VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT);
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
  template <typename T>
  resource<shaderProgram> loadShaderProg(const std::string& vertexShader, const std::string& fragmentShader);
  resource<shaderProgram> loadShaderProg();
  void setDefaultShaderProg(const std::string& vertexShader, const std::string& fragmentShader);
  void unloadShaders();
  void reloadShaders();

  // Miscellaneous
  resource<model> loadModel(const std::string& filepath);
  resource<texture> loadTexture(const std::set<std::string>& filepaths);
  resource<skybox> loadSkybox(const std::string& path, const std::string& extension);
};

#include "resource.tpp"
