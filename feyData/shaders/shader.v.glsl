#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 vertexUV;
layout (location = 2) in vec4 normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec2 UV;
out vec4 fragNormal;
out vec4 vertPos;

void main(void) {
     gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
     UV = vertexUV;
     fragNormal = normal;
     vertPos = position;
}
