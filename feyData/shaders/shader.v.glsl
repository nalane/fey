#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 vertexUV;
layout (location = 2) in vec4 normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec2 fragUV;
out vec4 fragNormal;
out vec4 fragPosition;

void main(void) {	
	fragUV = vertexUV;
    fragNormal = normalize(viewMatrix * modelMatrix * normal);
	fragPosition = viewMatrix * modelMatrix * position;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
}
