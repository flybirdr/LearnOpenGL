#version 330 core

layout (location = 0 ) in vec3 aPosition;
layout (location = 2 ) in vec2 aTexCoords;

out vec2 vTexCoords;

void main()
{
   gl_Position=vec4(aPosition,1.0);
   vTexCoords=aTexCoords;
}