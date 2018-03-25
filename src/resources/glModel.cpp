#include "glModel.hpp"
#include "glTexture.hpp"
#include "glShaderProgram.hpp"

using namespace std;

glModel::glModel(const string& name) : model(name) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Bind uniforms to shader
    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(modelUniforms), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);
}

// Removes all vbos and the vao
glModel::~glModel() {
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ubo);
    glDeleteVertexArrays(1, &vao);
}

void glModel::bindData() {
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(modelVertex), vertices.data(), GL_STATIC_DRAW);

    modelVertex::bindLayout();
}

void glModel::draw(modelUniforms uniforms) {
    glShaderProgram* prog = (glShaderProgram*)child_resources["shaderProgs"]["default"];
    prog->useProgram();

    glBindVertexArray(vao);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(uniforms), &uniforms);

    // Bind textures
    int index = 0;
    for (auto p : child_resources["textures"]) {
        glTexture* tex = (glTexture*)(p.second);
        tex->draw(index);
        index++;
    }

    // Draw vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}