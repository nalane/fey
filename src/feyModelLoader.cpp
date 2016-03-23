#include "feyModelLoader.hpp"

#include <fstream>
#include <map>
#include <cstdlib>
#include <string>

#include "log.hpp"
#include "glHeaders.hpp"

using namespace std;

model* loadFeyModel(string filename) {
  model* m = NULL;
  ifstream fin(filename.c_str());

  if (fin.is_open()) {
    int numVertices = 0;
    fin >> numVertices;

    vector<glm::vec3> vertexList;
    for (int i = 0; i < numVertices; i++) {
      float x, y, z;

      fin >> x;
      fin >> y;
      fin >> z;

      vertexList.push_back(glm::vec3(x, y, z));
    }
	
	int numTextures = 0;
	fin >> numTextures;
	
	vector<map<int, glm::vec2>> uvMaps;
	for (int i = 0; i < numTextures; i++) {
		string filename = "";
		int numUVCoords = 0;
		
		fin >> filename;
		fin >> numUVCoords;
		
		map<int, glm::vec2> uvMap;
		for (int j = 0; j < numUVCoords; j++) {
			int uvIdx;
			float u, v;
			
			fin >> uvIdx;
			fin >> u;
			fin >> v;
			
			uvMap[uvIdx] = glm::vec2(u, v);
		}
		
		uvMaps.push_back(uvMap);
	}

	int numVerts = 0;
	fin >> numVerts;
	numVerts /= 2;
	
    vector<int> indexList, uvIndexList;
    for(int i = 0; i < numVerts; i++) {
		int index, uvIndex;
		
		fin >> index;
		indexList.push_back(index);
		
		fin >> uvIndex;
		uvIndexList.push_back(uvIndex);
    }
    
    m = new model();
    m->setVertices(vertexList);
    m->setElementIndices(indexList);
  }

  else {
    recordLog("Could not read file: " + filename);
  }

  return m;
}
