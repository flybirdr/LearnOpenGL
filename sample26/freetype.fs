#version 330 core


in vec2 vTexCoords;
out vec4 vColor;

uniform sampler2D text;
uniform vec3 textColor;

void main(){
   vec4 sampled=vec4(1.0,1.0,1.0,texture(text,vTexCoords).r);
   vColor=vec4(textColor,1.0)*sampled;
}