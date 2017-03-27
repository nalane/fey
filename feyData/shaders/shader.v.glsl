#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 vertexUV;
layout (location = 2) in vec4 normal;

uniform mat4 modelMatrix;
uniform mat4 mvpMatrix;
uniform mat4 viewMatrix;

out vec2 fragUV;
out vec4 fragNormal;
out vec4 fragView;

void main(void) {	
	fragUV = vertexUV;
    fragNormal = normalize(modelMatrix * normal);	
	fragView = modelMatrix * position;
	gl_Position = mvpMatrix * position;
}
