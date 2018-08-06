#version 330

in vec4 positionColor;
out vec4 color;
//uniform sampler2D texture;

void main() {
//    color = texture2D(texture,  fUV.st);
    color = positionColor;
}
