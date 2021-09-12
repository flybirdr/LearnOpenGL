#version 330 core
out vec4 FragColor;

in vec3 vNormal;
in vec3 vFragPos;

uniform vec3 viewPos;

struct Light{
   vec3 position;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

struct Material {
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
   float shininess;
};

uniform Material material;
uniform Light light;

void main(){
   //法线、光源
   vec3 norm=normalize(vNormal);
   vec3 ligthDir = normalize(light.position - vFragPos);
   //环境光
   vec3 ambient = light.ambient * material.ambient;
   //漫反射
   float diff = max(dot(norm,ligthDir),0.0);
   vec3 diffuse = light.diffuse * (diff*material.diffuse);
   //镜面光
   vec3 viewDir=normalize(viewPos-vFragPos);
   vec3 reflectDir = reflect(-ligthDir,norm);
   float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
   vec3 specular = light.specular*(spec*material.specular);
   vec3 result = ambient + diffuse +specular;

   FragColor=vec4(result,1.0);
}