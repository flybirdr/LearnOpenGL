#ifndef SHADER_H
#define SHADER_H

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<glad/glad.h>

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
   void setMat4(const std::string &name, GLfloat *mat4) const;
   ~Shader();
};

Shader::Shader(const char *vertex_shader, const char *frag_shader)
{
   std::string vertextCode;
   std::string fragCode;
   std::ifstream vShaderFile;
   std::ifstream fShaderFile;
   // 保证ifstream对象可以抛出异常：
   vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
   fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

   try
   {
      vShaderFile.open(vertex_shader);
      fShaderFile.open(frag_shader);
      std::stringstream vShaderStream, fShaderStream;
      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();
      vShaderFile.close();
      fShaderFile.close();
      vertextCode = vShaderStream.str();
      fragCode = fShaderStream.str();
   }
   catch (std::ifstream::failure e)
   {
      std::cerr << e.what() << '\n';
   }
   const char *vShaderCode = vertextCode.c_str();
   const char *fShaderCode = fragCode.c_str();
   std::cout << vShaderCode << std::endl;
   std::cout << fShaderCode << std::endl;
   GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vShader, 1, &vShaderCode, NULL);
   glCompileShader(vShader);

   int success;
   char infoLog[512];
   glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      glGetShaderInfoLog(vShader, 512, NULL, infoLog);
      std::cout << "error: vtx shader compile failed:" << infoLog << std::endl;
   }

   GLuint fShader;
   fShader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fShader, 1, &fShaderCode, NULL);
   glCompileShader(fShader);
   glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
   if (!success)
   {
      glGetShaderInfoLog(fShader, 512, NULL, infoLog);
      std::cout << "error: frag shader compile failed:" << infoLog << std::endl;
   }

   mProgram = glCreateProgram();
   glAttachShader(mProgram, vShader);
   glAttachShader(mProgram, fShader);
   glLinkProgram(mProgram);

   glGetProgramiv(mProgram, GL_LINK_STATUS, &success);
   if (!success)
   {
      glGetProgramInfoLog(mProgram, 512, NULL, infoLog);
      std::cout << "link error:" << infoLog << std::endl;
   }
   glDeleteShader(vShader);
   glDeleteShader(fShader);
}

GLuint Shader::program()
{
   return mProgram;
}

void Shader::use()
{
   glUseProgram(mProgram);
}

void Shader::setBool(const std::string &name, bool value) const
{
   glUniform1i(glGetUniformLocation(mProgram, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const
{
   glUniform1i(glGetUniformLocation(mProgram, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const
{
   glUniform1f(glGetUniformLocation(mProgram, name.c_str()), value);
}
void Shader::setMat4(const std::string &name, GLfloat *mat4) const
{
   glUniformMatrix4fv(glGetUniformLocation(mProgram, name.c_str()), 1, GL_FALSE, mat4);
}
Shader::~Shader()
{
   glDeleteProgram(mProgram);
}
#endif