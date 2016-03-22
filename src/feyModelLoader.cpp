#include "feyModelLoader.hpp"

#include <fstream>
#include <vector>
#include <cstdlib>

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
    vector<glm::vec4> colorList;
    for (int i = 0; i < numVertices; i++) {
      float x, y, z;

      fin >> x;
      fin >> y;
      fin >> z;

      vertexList.push_back(glm::vec3(x, y, z));
      colorList.push_back(glm::vec4(rand() / (float) RAND_MAX,
				    rand() / (float) RAND_MAX,
				    rand() / (float) RAND_MAX, 1.0));
    }

    vector<int> indexList;
    int index;
    while (fin >> index) {
      indexList.push_back(index);
    }
    
    m = new model();
    m->setVertices(vertexList);
    m->setColors(colorList);
    m->setElementIndices(indexList);
  }

  else {
    recordLog("Could not read file: " + filename);
  }

  return m;
}
