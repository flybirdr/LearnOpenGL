#version 330 core
out vec4 FragColor;

in vec3 vNormal;
in vec3 vFragPos;

uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 ambientColor;
uniform vec3 viewPos;
uniform vec2 strength;

void main(){

   vec3 normal=normalize(vNormal);
   vec3 lightDir=normalize(lightPos-vFragPos);
   float diff=max(dot(normal,lightDir),0.0);
   vec3 diffuse=diff*lightColor;
   vec3 viewDir=normalize(viewPos-vFragPos);
   vec3 reflectDir=reflect(-lightDir,normal);
   float spec=pow(max(dot(viewDir,reflectDir),0.0),32);
   vec3 specular=strength.x*spec*lightColor;
   vec3 result=(ambientColor+diffuse+specular)*objectColor;

   FragColor=vec4(result,1.0);
}