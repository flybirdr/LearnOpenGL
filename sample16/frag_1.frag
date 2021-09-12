#version 330 core
out vec4 FragColor;

in vec3 vNormal;
in vec3 vFragPos;
in vec2 vTexCoord;

uniform vec3 viewPos;

struct SpotLight{
   vec3 position;
   vec3 direction;
   float innerCutoff;
   float outterCutoff;
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
   sampler2D shader;
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
   sampler2D diffuse;
   sampler2D specular;
   float shininess;
};

uniform Material material;
uniform Light light;
uniform SpotLight spotLight;
uniform DotLight dotLight;

vec4 calculateDirectionLight();
vec4 calculateDotLight();
vec4 calculateSpotLight();

void main(){
   vec4 dotLight1=calculateDotLight();
   vec4 directionLight=calculateDirectionLight();
   vec4 spotLight1=calculateSpotLight();
   // FragColor=spotLight1;
   FragColor=dotLight1+directionLight+spotLight1;
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
      vec3 ambient = spotLight.ambient * vec3(texture(material.specular,vTexCoord));
      //漫反射
      float diff = max(dot(norm,ligthDir),0.0);
      vec3 diffuse = spotLight.diffuse * diff*vec3(texture(material.diffuse,vTexCoord));
      //镜面光
      vec3 viewDir=normalize(viewPos-vFragPos);
      vec3 reflectDir = reflect(-ligthDir,norm);
      float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
      vec3 specular = spotLight.specular*spec*vec3(texture(material.specular, vTexCoord));
      diffuse *= intensity;
      specular *= intensity;
      vec3 result = ambient + diffuse +specular;
      result=vec3(texture(spotLight.shader,vTexCoord))*result;
      vec4 fragColor=vec4(result,1.0);
      return fragColor;
   }else{
      return vec4(spotLight.ambient * vec3(texture(material.specular,vTexCoord)),1.0f);
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
   vec3 ambient = dotLight.ambient* vec3(texture(material.specular,vTexCoord));
   //漫反射
   float diff = max(dot(norm,ligthDir),0.0);
   vec3 diffuse = dotLight.diffuse * diff*vec3(texture(material.diffuse,vTexCoord));
   //镜面光
   vec3 viewDir=normalize(viewPos-vFragPos);
   vec3 reflectDir = reflect(-ligthDir,norm);
   float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
   vec3 specular = dotLight.specular*spec*vec3(texture(material.specular, vTexCoord));
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

   vec4 fragColor=vec4(result,1.0);
   return fragColor;
}