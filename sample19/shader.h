#ifndef SHADER_H
#define SHADER_H

#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <string>

class Shader
{
private:
   GLuint mProgram;
   mutable int mUseCount;
   void incRef() const
   {
      mUseCount++;
   }
   void release()
   {
      if (--mUseCount == 0)
      {
         if (mProgram)
         {
            glDeleteProgram(mProgram);
            mProgram = 0;
         }
      }
   }

public:
   Shader(const char *vertex_shader, const char *frag_shader);
   Shader(const Shader &src)
   {
      if (&src == this)
      {
         return;
      }
      release();
      src.incRef();
      mProgram = src.mProgram;
      mUseCount = src.mUseCount;
   }
   Shader(const Shader &&src)
   {
      if (&src == this)
      {
         return;
      }
      release();
      src.incRef();
      mProgram = src.mProgram;
      mUseCount = src.mUseCount;
   }
   Shader &operator=(const Shader &src)
   {
      if (&src == this)
      {
         return *this;
      }
      release();
      src.incRef();
      mProgram = src.mProgram;
      mUseCount = src.mUseCount;
      return *this;
   }
   Shader &operator=(const Shader &&src)
   {
      if (&src == this)
      {
         return *this;
      }
      release();
      src.incRef();
      mProgram = src.mProgram;
      mUseCount = src.mUseCount;
      return *this;
   }
   GLuint program();
   void use();
   void setBool(const std::string &name, bool value) const;
   void setInt(const std::string &name, int value) const;
   void setFloat(const std::string &name, float value) const;
   void setMat4(const std::string &name, GLfloat *mat4) const;
   void setVec2(const std::string &name, float x, float y);
   void setVec3(const std::string &name, float x, float y, float z);
   void setVec3(const std::string &name, const float *v);
   ~Shader();
};

Shader::Shader(const char *vertex_shader, const char *frag_shader)
    : mProgram(0)
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
void Shader::setVec2(const std::string &name, float x, float y)
{
   glUniform2f(glGetUniformLocation(mProgram, name.c_str()), x, y);
}
void Shader::setVec3(const std::string &name, float x, float y, float z)
{
   glUniform3f(glGetUniformLocation(mProgram, name.c_str()), x, y, z);
}
void Shader::setVec3(const std::string &name, const float *v)
{
   glUniform3fv(glGetUniformLocation(mProgram, name.c_str()), 1, v);
}
Shader::~Shader()
{
   release();
}
#endif