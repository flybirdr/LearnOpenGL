#version 330 core
layout(location = 0) in vec3 aPostion;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 vTexCoords;

void main(){
   gl_Position=projection*view*model*vec4(aPostion,1.0);
   vTexCoords=vec2(aTexCoords.x,1-aTexCoords.y);
}