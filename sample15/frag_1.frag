#version 330 core
out vec4 FragColor;

in vec3 vNormal;
in vec3 vFragPos;
in vec2 vTexCoord;

uniform vec3 viewPos;

struct Light{
   vec3 position;
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

struct Material {
   sampler2D diffuse;
   sampler2D specular;
   float shininess;
};

uniform Material material;
uniform Light light;

void main(){
   //法线、光源
   vec3 norm=normalize(vNormal);
   vec3 ligthDir = normalize(light.position - vFragPos);
   //环境光
   vec3 ambient = light.ambient * vec3(texture(material.specular,vTexCoord));
   //漫反射
   float diff = max(dot(norm,ligthDir),0.0);
   vec3 diffuse = light.diffuse * diff*vec3(texture(material.diffuse,vTexCoord));
   //镜面光
   vec3 viewDir=normalize(viewPos-vFragPos);
   vec3 reflectDir = reflect(-ligthDir,norm);
   float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
   vec3 specular = light.specular*spec*vec3(texture(material.specular, vTexCoord));
   vec3 result = ambient + diffuse +specular;

   FragColor=vec4(result,1.0);
}