#version 330 core

#define MAX_LIGHTS 50

struct material {
     vec4 ambient;
     vec4 diffuse;
     vec4 specular;
};

struct light {
     vec4 position;
     vec3 color;
};

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 vertexUV;
layout (location = 2) in vec4 normal;

uniform material mat;
uniform int numLights;
uniform vec4 lights[MAX_LIGHTS];

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec2 UV;

void main(void) {
     gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
     UV = vertexUV;
}
