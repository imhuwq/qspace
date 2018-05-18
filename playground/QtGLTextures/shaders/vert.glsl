#version 330

layout(location=0) in vec3 vertex;
layout(location=1) in vec2 texCoord;

out vec2 texc;

uniform mediump mat4 matrix;

void main(void){
    gl_Position = matrix * vec4(vertex, 1.0);
    texc = texCoord;
}
