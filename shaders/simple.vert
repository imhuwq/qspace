#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;

out vec2 fUV;

uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 cameraToView;

void main() {
	gl_Position = cameraToView * worldToCamera * modelToWorld * vec4(position, 1.0);
	fUV = uv;
}
