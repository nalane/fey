#include "bulletDebug.h"
#include "log.hpp"
#include <string>

using namespace std;

bulletDebugDrawer::bulletDebugDrawer(resourceHandler* rHandler) : rHandler(rHandler) {
  string vertexShader = string("#version 330 core\n") +
    "layout (location = 0) in vec3 position;\n" + 
    "layout (location = 1) in vec3 color;\n" + 
    "out vec3 fcolor;\n" + 
    "void main(void) {\n" +
    "fcolor = color;" +
    "gl_Position = vec4(position, 1.0);" +
    "}";

  string fragShader = string("#version 330 core\n") +
    "in vec3 fcolor;\n" +
    "out vec4 color;\n" + 
    "void main(void) {\n" +
    "color = vec4(fcolor, 1.0);\n" +
    "}";

  GLuint vertexProg, fragProg;
  prog = glCreateProgram();
  vertexProg = glCreateShader(GL_VERTEX_SHADER);
  fragProg = glCreateShader(GL_FRAGMENT_SHADER);

  const char* vArr = vertexShader.c_str();
  const char* fArr = fragShader.c_str();
  
  glShaderSource(vertexProg, 1, &vArr, NULL);
  glCompileShader(vertexProg);
  GLint compiled = 0;
  glGetShaderiv(vertexProg, GL_COMPILE_STATUS, &compiled);
  if (!compiled) {
    recordLog("Debug drawer vertex shader failed to compile. Reason:");
    
    GLint maxLength = 0;
    glGetShaderiv(vertexProg, GL_INFO_LOG_LENGTH, &maxLength);
    
    char* message = new char[maxLength];
    glGetShaderInfoLog(vertexProg, maxLength, &maxLength, message);
    
    recordLog(message);
    delete[] message;
  }
  glAttachShader(prog, vertexProg);
  
  glShaderSource(fragProg, 1, &fArr, NULL);
  glCompileShader(fragProg);
  glGetShaderiv(fragProg, GL_COMPILE_STATUS, &compiled);
  if (!compiled) {
    recordLog("Debug drawer frag shader failed to compile. Reason:");
    
    GLint maxLength = 0;
    glGetShaderiv(fragProg, GL_INFO_LOG_LENGTH, &maxLength);
    
    char* message = new char[maxLength];
    glGetShaderInfoLog(fragProg, maxLength, &maxLength, message);
    
    recordLog(message);
    delete[] message;
  }
  glAttachShader(prog, fragProg);

  glLinkProgram(prog);

  glGenVertexArrays(1, &vao);
  glGenBuffers(2, vbos);
}

void bulletDebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) {
  glm::vec4 fVec = rHandler->getActiveCamera()->getProjectionMatrix() *
    rHandler->getActiveCamera()->getViewMatrix() *
    glm::vec4(from.getX(), from.getY(), from.getZ(), 1.0);
  glm::vec4 tVec = rHandler->getActiveCamera()->getProjectionMatrix() *
    rHandler->getActiveCamera()->getViewMatrix() *
    glm::vec4(to.getX(), to.getY(), to.getZ(), 1.0);
  
  btScalar vertices[6] = {
    fVec.x, fVec.y, fVec.z,
    tVec.x, tVec.y, tVec.z
  };

  //recordLog("From: " + to_string(fVec[0]) + ", " + to_string(fVec[1]) + ", " + to_string(fVec[2]));
  //recordLog("To: " + to_string(tVec[0]) + ", " + to_string(tVec[1]) + ", " + to_string(tVec[2]));

  btScalar colors[6] = {
    color.getX(), color.getY(), color.getZ(),
    color.getX(), color.getY(), color.getZ()
  };
  
  glUseProgram(prog);
  glBindVertexArray(vao);
  
  glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(btScalar), vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(btScalar), colors, GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
  glDrawArrays(GL_LINES, 0, 2);
}
