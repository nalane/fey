#include "skybox.hpp"
#include "resourceHandler.hpp"

using namespace std;

skybox::~skybox() {
  glDeleteVertexArrays(1, &vao);
  glDeleteTextures(1, &texID);
}

void skybox::setTextures(const char* texturePaths[NUM_SKYBOX_TEXTURES]) {
  // Generate vertices
  float data[] = {
    // Positions
    50.0f, -50.0f, -50.0f,
    -50.0f, -50.0f, -50.0f,
    -50.0f,  50.0f, -50.0f,
    -50.0f,  50.0f, -50.0f,
    50.0f,  50.0f, -50.0f,
    50.0f, -50.0f, -50.0f,

    -50.0f, -50.0f,  50.0f,
    -50.0f, -50.0f, -50.0f,
    -50.0f,  50.0f, -50.0f,
    -50.0f,  50.0f, -50.0f,
    -50.0f,  50.0f,  50.0f,
    -50.0f, -50.0f,  50.0f,

    50.0f, -50.0f, -50.0f,
    50.0f, -50.0f,  50.0f,
    50.0f,  50.0f,  50.0f,
    50.0f,  50.0f,  50.0f,
    50.0f,  50.0f, -50.0f,
    50.0f, -50.0f, -50.0f,

    -50.0f, -50.0f,  50.0f,
    -50.0f,  50.0f,  50.0f,
    50.0f,  50.0f,  50.0f,
    50.0f,  50.0f,  50.0f,
    50.0f, -50.0f,  50.0f,
    -50.0f, -50.0f,  50.0f,

    -50.0f,  50.0f, -50.0f,
    50.0f,  50.0f, -50.0f,
    50.0f,  50.0f,  50.0f,
    50.0f,  50.0f,  50.0f,
    -50.0f,  50.0f,  50.0f,
    -50.0f,  50.0f, -50.0f,

    -50.0f, -50.0f, -50.0f,
    -50.0f, -50.0f,  50.0f,
    50.0f, -50.0f, -50.0f,
    50.0f, -50.0f, -50.0f,
    -50.0f, -50.0f,  50.0f,
    50.0f, -50.0f,  50.0f
  };
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  
  // Generate texture
  glGenTextures(1, &texID);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

  // Load images to texture
  for (int i = 0; i < NUM_SKYBOX_TEXTURES; i++) {
    int width;
    int height;
    unsigned char* imageData = SOIL_load_image(texturePaths[i], &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
		 width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
  }

  // Set drawing methods
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 
}

void skybox::draw() {
  shaderProgram* prog = (shaderProgram*)(child_resources["shaderProg"]);
  glDepthMask(GL_FALSE);
  prog->useProgram();
  glm::mat4 viewMatrix = glm::mat4(glm::mat3(activeCamera->getViewMatrix()));
  glm::mat4 vpMatrix = activeCamera->getProjectionMatrix() * viewMatrix;
  GLint vpHandle = glGetUniformLocation(prog->getProgID(), "vpMatrix");
  glUniformMatrix4fv(vpHandle, 1, GL_FALSE, &vpMatrix[0][0]);
  
  glBindVertexArray(vao);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glDepthMask(GL_TRUE);
}
