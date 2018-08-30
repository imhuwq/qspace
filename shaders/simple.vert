#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec4 positionColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);

	positionColor = projectionMatrix * viewMatrix * modelMatrix * vec4(normal, 1.0);
}
