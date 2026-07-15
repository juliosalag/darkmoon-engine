#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec2 TextCoord;

uniform sampler2D texture0;
uniform vec4 customColor;

void main(){
   FragColor = texture(texture0, TextCoord) * customColor;
}