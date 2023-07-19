#ifndef SHADER_H
#define SHADER_H

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<GL/glew.h>


class Shader
{
private:
   GLuint mProgram;

public:
   Shader(const char* vertex_shader,const char* frag_shader);
   GLuint program();
   void use();
   void setBool(const std::string &name, bool value) const;
   void setInt(const std::string &name, int value) const;
   void setFloat(const std::string &name, float value) const;
   ~Shader();
};



#endif