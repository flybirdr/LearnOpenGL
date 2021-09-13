#version 330 core

in vec2 vTexCoords;
uniform sampler2D shader1;
out vec4 FragColor;

void main(){
   vec4 color=texture(shader1,vTexCoords);
   FragColor=color;
}