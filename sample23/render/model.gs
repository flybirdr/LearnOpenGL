#version 330 core

layout (triangles) in;
layout (triangle_strip,max_vertices = 3) out;

in VS_OUT{
 vec3 vNormal;
 vec3 vFragPos;
 vec2 vTexCoords; 
}gs_in[];

out VS_OUT{
 vec3 vNormal;
 vec3 vFragPos;
 vec2 vTexCoords; 
}gs_out;

uniform float time;

vec3 getNormal(){
   vec3 a=vec3(gl_in[0].gl_Position-gl_in[1].gl_Position);
   vec3 b=vec3(gl_in[2].gl_Position-gl_in[1].gl_Position);
   vec3 normal=normalize(cross(a,b));
   return normal;
}

vec4 explode(vec4 position,vec3 normal){
   float magnitude=2.0;
   vec3 direction=normal*((sin(time)+1.0)/2)*magnitude;
   return position+vec4(direction,0.0);
}

void main(){
   vec3 normal=getNormal();

   gl_Position=explode(gl_in[0].gl_Position,normal);
   gs_out.vTexCoords=gs_in[0].vTexCoords;
   gs_out.vFragPos=vec3(gl_Position);
   gs_out.vNormal=gs_in[0].vNormal;
   EmitVertex();

   gl_Position=explode(gl_in[1].gl_Position,normal);
    gs_out.vTexCoords=gs_in[1].vTexCoords;
   gs_out.vFragPos=vec3(gl_Position);
   gs_out.vNormal=gs_in[1].vNormal;
   EmitVertex();

   gl_Position=explode(gl_in[2].gl_Position,normal);
    gs_out.vTexCoords=gs_in[2].vTexCoords;
   gs_out.vFragPos=vec3(gl_Position);
   gs_out.vNormal=gs_in[2].vNormal;
   EmitVertex();

   EndPrimitive();
}