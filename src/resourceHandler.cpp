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
#include "graphics.hpp"
#include "vkShaderProgram.hpp"
#include "modelVertex.hpp"

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
    m = model::createModel(filename);

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

        texture* newTexture = texture::createTexture(filename, {filename});
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
    vector<modelVertex> finalVerts;
    glm::vec3 face[3];
    for(int i = 0; i < numVerts; i++) {
      int index, uvIndex;

      finalVerts.push_back(modelVertex());
			
      fin >> index;
      finalVerts[i].position = glm::vec4(vertexList[index], 1.0);

      fin >> uvIndex;
      if (uvCoords.size() > 0) {
        finalVerts[i].vertexUV = uvCoords[uvIndex];
      }

      // For early versions of the model spec, set normal based on last three positions.
      if (version.substr(0, 1) != "v") {
        face[i % 3] = vertexList[index];
        // Set the normal after collecting three vertices
        if (i % 3 == 2) {
          glm::vec3 normal = glm::cross(face[1] - face[0], face[2] - face[0]);
          finalVerts[i].normal = glm::vec4(normal, 0.0);
          finalVerts[i - 1].normal = glm::vec4(normal, 0.0);
          finalVerts[i - 2].normal = glm::vec4(normal, 0.0);
        }
      }

      else {
        fin >> finalVerts[i].normal.x;
        fin >> finalVerts[i].normal.y;
        fin >> finalVerts[i].normal.z;
      }
    }

    // Push data into the model
    m->setVertices(finalVerts);
    
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

    // Get paths to shader programs
    string vertexShader = "";
    string fragmentShader = "";
    if (newModel->getNumTextures() == 0) {
      vertexShader = getDataFolderPath("shaders/coloredModel/coloredModel.vert");
      fragmentShader = getDataFolderPath("shaders/coloredModel/coloredModel.frag");
    } else {
      vertexShader = getDataFolderPath("shaders/texturedModel/texturedModel.vert");
      fragmentShader = getDataFolderPath("shaders/texturedModel/texturedModel.frag");
    }

    // Add child shader
    map<string, string> shaders = {
      {"vertex", vertexShader},
      {"fragment", fragmentShader}
    };
    string shaderKey = getShaderKey(shaders);
    if (resources.find(shaderKey) == resources.end()) {
      resources[shaderKey] = newShader<modelVertex>(shaders);
    }   
    newModel->setShaderProg((shaderProgram*)resources[shaderKey]);

    // Bind model data to GPU
    newModel->bindData();

    resources[filepath] = newModel;
  }

  return resource<model>((model*) resources[filepath]);
}

string resourceHandler::getShaderKey(const std::map<std::string, std::string>& shaders) const {
  string key = "";
  for (auto p : shaders) {
    key += p.first;
    key += p.second;
  }

  return key;
}

// Create a new shader program
template <typename T>
shaderProgram* resourceHandler::newShader(const map<string, string>& shaders, bool depthEnable, bool cullModeBackFaces) {
  string key = getShaderKey(shaders);
  recordLog("Loading shader " + key);

  shaderProgram* prog = shaderProgram::createShaderProgram(key, shaders);
  if (graphics::getInstance()->getLibrary() == VULKAN) {
    vkShaderProgram* vkprog = (vkShaderProgram*)prog;
    vkprog->setVertexAttributes<T>();
  }
  if (!prog->loadShaders(depthEnable, cullModeBackFaces)) {
    recordLog("ERROR: Could not read in shader " + key);
    return nullptr;
  }

  recordLog("Successfully read in shader " + key);
  return prog;
}

// Get a shader program
template <typename T>
resource<shaderProgram> resourceHandler::loadShaderProg(const map<string, string>& shaders) {
  string key = getShaderKey(shaders);
  if (resources.find(key) == resources.end()) {
    shaderProgram* shader = newShader<T>(shaders);
    resources[key] = shader;
    shaders[key] = shader;
  }

  return resource<shaderProgram>((shaderProgram*) resources[key]);
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
    string skyboxTextures[NUM_SKYBOX_TEXTURES];
    skyboxTextures[SKYBOX_RIGHT]   =  path + "/right."  + extension;
    skyboxTextures[SKYBOX_LEFT]    =  path + "/left."   + extension;
    skyboxTextures[SKYBOX_TOP]     =  path + "/top."    + extension;
    skyboxTextures[SKYBOX_BOTTOM]  =  path + "/bottom." + extension;
    skyboxTextures[SKYBOX_FRONT]   =  path + "/front."  + extension;
    skyboxTextures[SKYBOX_BACK]    =  path + "/back."   + extension;

    // Create textures key from filepath list
    string key = "";
    for (string s : skyboxTextures) {
      key += s;
    }

    // Find or create texture
    GraphicsLibrary lib = graphics::getInstance()->getLibrary();
    texture* tex = nullptr;
    if (lib == VULKAN && resources.find(key) == resources.end()) {
      recordLog("Loading texture " + key);
    
      tex = texture::createTexture(key, 
        set<string>(skyboxTextures, skyboxTextures + NUM_SKYBOX_TEXTURES));
      if (tex->loadTexture()) {
        resources[key] = tex;
        recordLog("Successfully read in texture " + key);
      }
      else {
        recordLog("ERROR: Could not read in texture " + key);
      }

      resources[key] = tex; 
    }

    // Get shader
    const map<string, string> shaders = {
      {"vertex", getDataFolderPath("shaders/skybox/skybox.vert")},
      {"fragment", getDataFolderPath("shaders/skybox/skybox.frag")}
    };
    string shaderKey = getShaderKey(shaders);
    if (resources.find(shaderKey) == resources.end()) {
      resources[shaderKey] = newShader<skyboxVertex>(shaders, false, false);
    }

    skybox* newSkybox = skybox::createSkybox(path);
    newSkybox->setShaderProg((shaderProgram*)resources[shaderKey]);
    newSkybox->setTextures(tex, skyboxTextures);
    newSkybox->bindData();
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
    
    texture* newTexture = texture::createTexture(key, filepaths);
    newTexture->loadTexture();
    resources[key] = newTexture;
  }

  return resource<texture>((texture*) resources[key]);
}

resource<terrain> resourceHandler::loadTerrain(const string& path) {
  string fullPath = getLibraryFolderPath(path);
  map<string, raw_resource*>::iterator it = resources.find(fullPath);
  if (it == resources.end()) {
    recordLog("Loading terrain: " + fullPath);

    // Get shader
    const map<string, string> shaders = {
      {"vertex", getDataFolderPath("shaders/terrain/terrain.vert")},
      {"fragment", getDataFolderPath("shaders/terrain/terrain.frag")},
      {"tess_control", getDataFolderPath("shaders/terrain/terrain.tesc")},
      {"tess_eval", getDataFolderPath("shaders/terrain/terrain.tese")}
    };
    string shaderKey = getShaderKey(shaders);
    if (resources.find(shaderKey) == resources.end()) {
      resources[shaderKey] = newShader<skyboxVertex>(shaders);
    }

    terrain* newTerrain = terrain::createTerrain(fullPath);

    // Open and load file
    ifstream fin(fullPath.c_str());
    if (!fin) {
        recordLog("ERROR: Could not open file " + fullPath);
        delete newTerrain;
        return resource<terrain>(nullptr);
    }

    // Each patch is a square; get the number of vertices on each side
    int patchSideLength = round(sqrt(terrain::PATCH_SIZE));

    // Get the width and height of the terrain
    int width, height;
    fin >> width >> height;
    if (width % patchSideLength != 0 || height % patchSideLength != 0) {
        recordLog("ERROR: Could not find a correct number of vertices in " + fullPath);
    }

    // Load control points
    vector<vector<terrainVertex>> vertices(height);
    for (vector<terrainVertex>& v : vertices) {
        v.resize(width);
        for (terrainVertex& point : v)
            fin >> point.position.x >> point.position.y >> point.position.z;
    }

    // Divide vertices into patches
    vector<terrainVertex> controlPoints;
    for (int i = 0; i < height; i += patchSideLength) {
        for (int j = 0; j < width; j += patchSideLength) {
            for (int k = 0; k < terrain::PATCH_SIZE; k++) {
                int rowNum = i + k / patchSideLength;
                int colNum = j + k % patchSideLength;
                controlPoints.push_back(vertices[rowNum][colNum]);
            }
        }
    }
    newTerrain->setVertices(controlPoints);

    // Get texture path
    bool hasTexture;
    fin >> hasTexture;

    // Load texture without wrapping in resource
    if (hasTexture) {
      string texturePath;
      fin >> texturePath;

      texturePath = getLibraryFolderPath(texturePath);
      map<string, raw_resource*>::iterator it = resources.find(texturePath);
      if (it == resources.end()) {
        recordLog("Loading texture " + texturePath);

        texture* newTexture = texture::createTexture(texturePath, {texturePath});
        if (newTexture->loadTexture()) {
          resources[texturePath] = newTexture;
          recordLog("Successfully read in texture " + texturePath);
        }
        else {
          recordLog("ERROR: Could not read in texture " + texturePath);
        }
      }
      newTerrain->setTexture((texture*)resources[texturePath]);
    }
    fin.close();

    newTerrain->setShaderProg((shaderProgram*)resources[shaderKey]);
    newTerrain->bindData();
    resources[fullPath] = newTerrain;
  }

  return resource<terrain>((terrain*) resources[fullPath]);
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