#include "resourceHandler.hpp"

#include <iostream>
#include <fstream>
#include <map>
#include <cstdlib>
#include <string>
#include <set>

#include "paths.hpp"
#include "log.hpp"
#include "glHeaders.hpp"

using namespace std;

// Singleton
resourceHandler* resourceHandler::instance;

// Create singleton instance
void resourceHandler::createInstance() {
  instance = new resourceHandler();
}

// Get singleton instance
resourceHandler* resourceHandler::getInstance() {
  return instance;
}

// Delete singleton instance
void resourceHandler::endInstance() {
  delete instance;
}

resourceHandler::~resourceHandler() {
  unloadAll();
}

// Loads a fey model into memory and returns the data
model* resourceHandler::loadFeyModel(const string& filename) {
  string fullPath = getLibraryFolderPath(filename);
  recordLog("Reading fey model " + fullPath + "...");
  model* m = nullptr;

  ifstream fin(fullPath.c_str());
  if (fin.is_open()) {
    m = new model(filename);
    

    // Get version number and number of materials
    string version = "";
    int numMaterials = 0;
    fin >> version;
    if (version.substr(0, 1) == "v") {
      fin >> numMaterials;
    }
    else {
      numMaterials = stoi(version);
    }
    
    // Get materials
    for (int i = 0; i < numMaterials; i++) {
      glm::vec4 amb(0.0);
      glm::vec4 diffuse(1.0);
      glm::vec4 specular(1.0);
      float specularIntensity;

      fin >> diffuse[0] >> diffuse[1] >> diffuse[2];
      fin >> specular[0] >> specular[1] >> specular[2];
      fin >> specularIntensity;

      m->addMaterial(material(amb, diffuse, specular, specularIntensity));
    }
    
    // Get number of vertices
    int numVertices = 0;
    fin >> numVertices;

    // Read all vertices
    vector<glm::vec3> vertexList;
    for (int i = 0; i < numVertices; i++) {
      float x, y, z;

      fin >> x;
      fin >> y;
      fin >> z;

      vertexList.push_back(glm::vec3(x, y, z));
    }
		
    // Get the number of textures
    int numTextures = 0;
    fin >> numTextures;

    // Get the texture data for each texture
    vector<glm::vec2> uvCoords;
    for (int i = 0; i < numTextures; i++) {
      string filename = "junk";
      int numUVCoords = -1;
			
      getline(fin, filename);
      getline(fin, filename);
      fin >> numUVCoords;
			
      for (int j = 0; j < numUVCoords; j++) {
	      int uvIdx;
	      float u, v;
				
	      fin >> uvIdx;
	      fin >> u;
        fin >> v;

        uvCoords.push_back(glm::vec2(u, v));
      }
		
      // Load texture without wrapping in resource
      filename = getLibraryFolderPath(filename);
      map<string, raw_resource*>::iterator it = resources.find(filename);
      if (it == resources.end()) {
        recordLog("Loading texture " + filename);

        texture* newTexture = new texture(filename, {filename});
        if (newTexture->loadTexture()) {
          resources[filename] = newTexture;
          recordLog("Successfully read in texture " + filename);
        }
        else {
          recordLog("ERROR: Could not read in texture " + filename);
        }
      }
      m->setTexture((texture*)resources[filename]);
    }

    // Number of vertices in final model
    int numVerts = 0;
    fin >> numVerts;
    int factor = version.substr(0, 1) == "v" ? 5 : 2;
    numVerts /= factor;
		
    // Get the UV map
    vector<glm::vec2> uvMapping;
    vector<glm::vec3> finalVerts;
    vector<glm::vec3> normals;
    glm::vec3 face[3];
    for(int i = 0; i < numVerts; i++) {
      int index, uvIndex;
			
      fin >> index;
      finalVerts.push_back(vertexList[index]);

      fin >> uvIndex;
      if (uvCoords.size() > 0) {
        uvMapping.push_back(uvCoords[uvIndex]);
      }

      if (version.substr(0, 1) != "v") {
        face[i % 3] = vertexList[index];
        if (i % 3 == 2) {
          glm::vec3 normal = glm::cross(face[1] - face[0], face[2] - face[0]);
          normals.push_back(normal);
          normals.push_back(normal);
          normals.push_back(normal);
        }
      }

      else {
        glm::vec3 normal;
        fin >> normal.x >> normal.y >> normal.z;
        normals.push_back(normal);
      }
    }

    // Push data into the model
    m->setVertices(finalVerts);
    m->setNormals(normals);
    if (uvMapping.size() > 0)
      m->setUVMapping(uvMapping);
    
    recordLog("Successfully read in fey model file " + filename + "!");
  }

  else {
    recordLog("Could not read file: " + filename);
  }

  return m;
}

// Get the model associated with the given filename
resource<model> resourceHandler::loadModel(const string& filepath) {
  if (resources.find(filepath) == resources.end()) {
    model* newModel = loadFeyModel(filepath);

    // Add shader child
    string shaderKey = getShaderKey(defaultVertexShader, defaultFragmentShader);
    if (resources.find(shaderKey) == resources.end()) {
      resources[shaderKey] = newShader<modelVertex>(defaultVertexShader, defaultFragmentShader, shaderKey);
    }   
    newModel->setShaderProgram((shaderProgram*)resources[shaderKey]);

    // Bind model data to GPU
    newModel->bindVertices();
    newModel->bindDescriptors();

    resources[filepath] = newModel;
  }

  return resource<model>((model*) resources[filepath]);
}

string resourceHandler::getShaderKey(const string& vert, const string& frag) {
  return "v" + vert + "f" + frag;
}

// Create a new shader program
template <typename T>
shaderProgram* resourceHandler::newShader(const string& vertexShader, const string& fragmentShader, const string& key) {
  recordLog("Loading shader " + key);
  map<string, string> shaderFiles;
  shaderFiles["vertex"] = vertexShader + ".spv";
  shaderFiles["fragment"] = fragmentShader + ".spv";

  shaderProgram* prog = new shaderProgram(key, shaderFiles);
  prog->setVertexAttributes<T>();
  if (!prog->loadShaders()) {
    recordLog("ERROR: Could not read in shader " + key);
    return nullptr;
  }

  recordLog("Successfully read in shader " + key);
  return prog;
}

// Get a shader program
template <typename T>
resource<shaderProgram> resourceHandler::loadShaderProg(const string& vertexShader, const string& fragmentShader) {
  string key = getShaderKey(vertexShader, fragmentShader);
  if (resources.find(key) == resources.end()) {
    shaderProgram* shader = newShader<T>(vertexShader, fragmentShader, key);
    resources[key] = shader;
    shaders[key] = shader;
  }

  return resource<shaderProgram>((shaderProgram*) resources[key]);
}

// Find the default shader program, if it is set
resource<shaderProgram> resourceHandler::loadShaderProg() {
  return loadShaderProg<modelVertex>(defaultVertexShader, defaultFragmentShader);
}

void resourceHandler::setDefaultShaderProg(const string& vertexShader, const string& fragmentShader) {
  defaultVertexShader = vertexShader;
  defaultFragmentShader = fragmentShader;
}

// Unload shaders when recreating swapchain
void resourceHandler::unloadShaders() {
  for (auto p : shaders) {
    p.second->unloadShaders();
  }
}

// Reload shaders for recreating swapchain
void resourceHandler::reloadShaders() {
  for (auto p : shaders) {
    p.second->loadShaders();
  }
}

resource<skybox> resourceHandler::loadSkybox(const string& path, const string& extension) {
  map<string, raw_resource*>::iterator it = resources.find(path);
  if (it == resources.end()) {
    recordLog("Loading skybox " + path);
    
    // Get the textures
    /*
    vector<string> skyboxTextures(NUM_SKYBOX_TEXTURES);
    skyboxTextures[SKYBOX_RIGHT]  = (path + "/right." + extension);
    skyboxTextures[SKYBOX_LEFT]   = (path + "/left." + extension);
    skyboxTextures[SKYBOX_TOP]    = (path + "/top." + extension);
    skyboxTextures[SKYBOX_BOTTOM] = (path + "/bottom." + extension);
    skyboxTextures[SKYBOX_BACK]   = (path + "/back." + extension);
    skyboxTextures[SKYBOX_FRONT]  = (path + "/front." + extension);
    */

    set<string> skyboxTextures;
    skyboxTextures.insert(path + "/right." + extension);
    skyboxTextures.insert(path + "/left." + extension);
    skyboxTextures.insert(path + "/top." + extension);
    skyboxTextures.insert(path + "/bottom." + extension);
    skyboxTextures.insert(path + "/back." + extension);
    skyboxTextures.insert(path + "/front." + extension);

    // Create textures key from filepath list
    string key = "";
    for (string s : skyboxTextures) {
      key += s;
    }

    // Find or create texture
    if (resources.find(key) == resources.end()) {
      recordLog("Loading texture " + key);
    
      texture* newTexture = new texture(key, skyboxTextures);
      if (newTexture->loadTexture()) {
        resources[key] = newTexture;
        recordLog("Successfully read in texture " + key);
      }
      else {
        recordLog("ERROR: Could not read in texture " + key);
      }

      resources[key] = newTexture; 
    }

    // Get shader
    string vertexShader = getDataFolderPath("shaders/skybox/skybox.vert");
    string fragShader = getDataFolderPath("shaders/skybox/skybox.frag");
    string shaderKey = getShaderKey(vertexShader, fragShader);
    if (resources.find(shaderKey) == resources.end()) {
      resources[shaderKey] = newShader<skyboxVertex>(vertexShader, fragShader, shaderKey);
    }

    skybox* newSkybox = new skybox(path);
    newSkybox->setShaderProgram((shaderProgram*)resources[shaderKey]);
    newSkybox->setTextures((texture*)resources[key]);
    resources[path] = newSkybox;

    recordLog("Successfully read in skybox " + path);
  }

  return resource<skybox>((skybox*) resources[path]);
}

resource<texture> resourceHandler::loadTexture(const set<string>& filepaths) {
  // Create key from filepath list
  string key = "";
  for (string s : filepaths) {
    key += s;
  }

  map<string, raw_resource*>::iterator it = resources.find(key);
  if (it == resources.end()) {
    recordLog("Loading texture " + key);
    
    texture* newTexture = new texture(key, filepaths);
    newTexture->loadTexture();
    resources[key] = newTexture;
  }

  return resource<texture>((texture*) resources[key]);
}

// Unload the named resource
void resourceHandler::unload(const string& name) {
  map<string, raw_resource*>::iterator it = resources.find(name);
  if (it != resources.end()) {
    // Delete from shaders map, if it exists there.
    map<string, shaderProgram*>::iterator shaderIt = shaders.find(name);
    if (shaderIt != shaders.end()) {
      shaders.erase(shaderIt);
    }

    // Delete resource
    recordLog("Unloading resource " + name);
    delete it->second;
    resources.erase(it);
  }
  else {
    recordLog("WARNING: Could not find resource " + name + " for unloading.");
  }
}

// Remove all resources
void resourceHandler::unloadAll() {
  shaders.clear();
  for (auto p : resources) 
    delete p.second;
  resources.clear();
}