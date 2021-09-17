#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;


layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};
uniform mat4 model;

out VS_OUT{
 vec3 vNormal;
 vec3 vFragPos;
 vec2 vTexCoords; 
}vs_out;



void main(){
   gl_Position=projection*view*model* vec4(aPos,1.0);
   vs_out.vFragPos=vec3(model*vec4(aPos,1.0));
   vs_out.vNormal=aNormal;
   vs_out.vTexCoords=aTexCoord;
}