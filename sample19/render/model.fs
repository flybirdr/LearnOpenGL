#version 330 core

in vec3 vNormal;
in vec3 vFragPos;
in vec2 vTexCoord;

uniform vec3 viewPos;
uniform vec2 singleColor;

struct SpotLight{
   vec3 position;
   vec3 direction;
   float innerCutoff;
   float outterCutoff;
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

struct DotLight{
   vec3 position;
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
   float constant;
   float linear;
   float quadratic;
};

struct Light{
   //定向光
   vec3 direction;
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

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
uniform Light light;
uniform SpotLight spotLight;
uniform DotLight dotLight;

vec4 calculateDirectionLight();
vec4 calculateDotLight();
vec4 calculateSpotLight();

out vec4 FragColor;


void main(){
   if(singleColor.x == 0.0){
      vec4 dotLight1=calculateDotLight();
      vec4 directionLight=calculateDirectionLight();
      vec4 spotLight1=calculateSpotLight();
      FragColor=directionLight+dotLight1+spotLight1;
   }else{
      FragColor=vec4(0.0,0.0,1.0,1.0);
   }
   
}

vec4 calculateSpotLight(){
   vec3 lightDir=normalize(spotLight.position-vFragPos);
   float theta     = dot(lightDir, normalize(-spotLight.direction));
   float epsilon   = spotLight.innerCutoff - spotLight.outterCutoff;
   float intensity = clamp((theta - spotLight.outterCutoff) / epsilon, 0.0, 1.0); 
   if(theta<spotLight.outterCutoff){
      //法线、光源
      vec3 norm=normalize(vNormal);
      vec3 ligthDir = normalize(spotLight.position-vFragPos);
      //环境光
      vec3 ambient = spotLight.ambient * vec3(texture(material.texture_specular1,vTexCoord));
      //漫反射
      float diff = max(dot(norm,ligthDir),0.0);
      vec3 diffuse = spotLight.diffuse * diff*vec3(texture(material.texture_diffuse1,vTexCoord));
      //镜面光
      vec3 viewDir=normalize(viewPos-vFragPos);
      vec3 reflectDir = reflect(-ligthDir,norm);
      float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
      vec3 specular = spotLight.specular*spec*vec3(texture(material.texture_specular1, vTexCoord));
      diffuse *= intensity;
      specular *= intensity;
      vec3 result = ambient + diffuse +specular;
      vec4 fragColor=vec4(result,1.0);
      return fragColor;
   }else{
      return vec4(spotLight.ambient * vec3(texture(material.texture_specular1,vTexCoord)),1.0f);
   }

}

vec4 calculateDotLight(){
   float distance= length(dotLight.position-vFragPos);
   float attenuation = 1.0 / (dotLight.constant + dotLight.linear * distance + 
                dotLight.quadratic * (distance * distance));
   //法线、光源
   vec3 norm=normalize(vNormal);
   vec3 ligthDir = normalize(dotLight.position-vFragPos);
   //环境光
   vec3 ambient = dotLight.ambient* vec3(texture(material.texture_specular1,vTexCoord));
   //漫反射
   float diff = max(dot(norm,ligthDir),0.0);
   vec3 diffuse = dotLight.diffuse * diff*vec3(texture(material.texture_diffuse1,vTexCoord));
   //镜面光
   vec3 viewDir=normalize(viewPos-vFragPos);
   vec3 reflectDir = reflect(-ligthDir,norm);
   float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
   vec3 specular = dotLight.specular*spec*vec3(texture(material.texture_specular1, vTexCoord));
   ambient*=attenuation;
   diffuse*=attenuation;
   specular*=attenuation;
   vec3 result = ambient + diffuse +specular;
   vec4 fragColor=vec4(result,1.0);
   return fragColor;
}

vec4 calculateDirectionLight(){
   //法线、光源
   vec3 norm=normalize(vNormal);
   vec3 ligthDir = normalize(-light.direction);
   //环境光
   vec3 ambient = light.ambient * vec3(texture(material.texture_specular1,vTexCoord));
   //漫反射
   float diff = max(dot(norm,ligthDir),0.0);
   vec3 diffuse = light.diffuse * diff*vec3(texture(material.texture_diffuse1,vTexCoord));
   //镜面光
   vec3 viewDir=normalize(viewPos-vFragPos);
   vec3 reflectDir = reflect(-ligthDir,norm);
   float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
   vec3 specular = light.specular*spec*vec3(texture(material.texture_specular1, vTexCoord));
   vec3 result = ambient + diffuse +specular;
   vec4 fragColor=vec4(result,1.0);
   return fragColor;
}