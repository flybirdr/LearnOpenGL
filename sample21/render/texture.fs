#version 330 core

in vec2 vTexCoords;
uniform sampler2D uTexture;

out vec4 vFragColor;

void main()
{
   vFragColor=texture(uTexture,vTexCoords);
}