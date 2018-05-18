#version 330

in vec2 texc;

uniform sampler2D texture;

void main(void){
    gl_FragColor = texture2D(texture, texc.st);
}
