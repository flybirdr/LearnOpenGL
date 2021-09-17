#version 330 core

layout(location =0 ) in vec3 aPosition;
layout(location =1 ) in vec3 aColor;

layout(std140) uniform Matrices
{
   mat4 projection;
   mat4 view;
};

out VS_OUT{
   vec3 color;
}vs_out;

void main(){

   gl_Position=projection*view* vec4(aPosition,1.0);
   vs_out.color=aColor;
}