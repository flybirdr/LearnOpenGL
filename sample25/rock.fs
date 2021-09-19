#version 330 core

in vec2 vTexCoords;

struct Material {
   sampler2D texture_diffuse1;
   sampler2D texture_diffuse2;
   sampler2D texture_diffuse3;
   sampler2D texture_diffuse4;
   sampler2D texture_specular1;
   sampler2D texture_specular2;
   sampler2D texture_specular3;
   sampler2D texture_specular4;

   float shininess;
};

uniform Material material;

out vec4 vColor;

void main(){

   vColor=texture(material.texture_diffuse1,vTexCoords);
}