#include "glShaderProgram.hpp"
#include "log.hpp"

#include <fstream>
#include <exception>

using namespace std;

map<string, GLenum> shaderTypes = {
    {"vertex", GL_VERTEX_SHADER},
    {"fragment", GL_FRAGMENT_SHADER},
    {"tess_control", GL_TESS_CONTROL_SHADER},
    {"tess_eval", GL_TESS_EVALUATION_SHADER}
};

glShaderProgram::~glShaderProgram() {
    unloadShaders();
}

void glShaderProgram::loadShaders(bool depthEnable, bool cullModeBackFaces) {
    for (auto p : shaderPaths) {
        GLenum type = shaderTypes[p.first];
        string filename = p.second;

        // Open shader file
        ifstream fin(filename.c_str(), ios::ate);
        if (!fin.is_open()) {
            error("ERROR: Could not open shader file " + p.second);
        }
        size_t fileSize = fin.tellg();
        vector<char> buffer(fileSize);

        // Read in data from the file
        fin.seekg(0);
        fin.read(buffer.data(), fileSize);
        fin.close();

        // Compile shader
        const char* data[] = {buffer.data()};
        GLuint shaderID = glCreateShader(type);
        glShaderSource(shaderID, 1, data, nullptr);
        glCompileShader(shaderID);

        // Check if it's really compiled
        GLint compiled = 0;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint maxLength = 0;
            glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);
    
            char* message = new char[maxLength];
            glGetShaderInfoLog(shaderID, maxLength, &maxLength, message);
    
            string msg = "Compiling " + p.first + " shader failed with the following message:\n" + message;
            delete[] message;

            error(msg);
        }
        else {
            recordLog("Successfully compiled " + p.first + " shader!");
        }

        shaders[type] = shaderID;
    }

    // Link shaders
    progID = glCreateProgram();
    for (auto p : shaders) {
        glAttachShader(progID, p.second);
    }
    glLinkProgram(progID);

    // Make sure it really is linked
    GLint isLinked = 0;
    glGetProgramiv(progID, GL_LINK_STATUS, &isLinked);
    if (!isLinked) {
        GLint maxLength = 0;
        glGetProgramiv(progID, GL_INFO_LOG_LENGTH, &maxLength);
    
        char* message = new char[maxLength];
        glGetProgramInfoLog(progID, maxLength, &maxLength, message);
    
        string msg = "Linking shader program failed with the following message:\n" + string(message);
        delete[] message;

        error(msg);
    }
    else {
        recordLog("Successfully linked all shaders!");
    }
}

void glShaderProgram::unloadShaders() {
    glDeleteProgram(progID);
    for (auto p : shaders) {
        glDeleteShader(p.second);
    }
}