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
   Shader(const std::string &vertex_shader, const std::string &frag_shader);
   Shader(const std::string &vertex_shader, const std::string &geometry_shader, const std::string &frag_shader);
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

private:
   std::string readFile(const std::string fileName);
   GLuint compileShader(GLenum type, const std::string &shader);
   GLuint createProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragShader);
};

Shader::Shader(const std::string &vertex_shader, const std::string &frag_shader)
    : mProgram(0)
{
 
   const std::string& vShaderCode = readFile(vertex_shader);
   const std::string& fShaderCode = readFile(frag_shader);

   // std::cout << vShaderCode << std::endl;
   // std::cout << fShaderCode << std::endl;

   GLuint vShader = compileShader(GL_VERTEX_SHADER,vShaderCode);
   GLuint fShader = compileShader(GL_FRAGMENT_SHADER, fShaderCode);

   mProgram = createProgram(vShader, 0, fShader);
}
Shader::Shader(const std::string &vertex_shader, const std::string &geometry_shader, const std::string &frag_shader)
{
   std::string vtxShaderSrc = readFile(vertex_shader);
   std::string geoShaderSrc = readFile(geometry_shader);
   std::string fragShaderSrc = readFile(frag_shader);

   // std::cout << vtxShaderSrc << std::endl;
   // std::cout << geoShaderSrc << std::endl;
   // std::cout << fragShaderSrc << std::endl;

   GLuint vtxShader = compileShader(GL_VERTEX_SHADER, vtxShaderSrc);
   GLuint geoShader = compileShader(GL_GEOMETRY_SHADER, geoShaderSrc);
   GLuint fragShader = compileShader(GL_FRAGMENT_SHADER, fragShaderSrc);
  
   mProgram = createProgram(vtxShader,geoShader,fragShader);
}

GLuint Shader::createProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragShader){
   GLuint program = glCreateProgram();
   std::cout << "created program:" << program << std::endl;

   glAttachShader(program, vertexShader);
   if (geometryShader)
   {
      glAttachShader(program, geometryShader);
   }
   glAttachShader(program, fragShader);

   glLinkProgram(program);

   GLint success;
   glGetProgramiv(program, GL_LINK_STATUS, &success);

   if (!success)
   {
      GLchar info[512];
      glGetProgramInfoLog(program, 512, 0, info);
      std::cerr
          << "failed to link program:" << program << " " << info << std::endl;
   }

   glDeleteShader(vertexShader);
   glDeleteShader(geometryShader);
   glDeleteShader(fragShader);
   return program;
}

GLuint Shader::compileShader(GLenum type, const std::string &shader)
{
   GLuint glShader = glCreateShader(type);
   const GLchar *shaderSource = shader.c_str();
   glShaderSource(glShader, 1, &shaderSource, 0);
   glCompileShader(glShader);
   GLint success;
   GLchar info[512];
   glGetShaderiv(glShader, GL_COMPILE_STATUS, &success);

   if (!success)
   {
      glGetShaderInfoLog(glShader, 512, 0, info);
      std::cerr << "failed to compile shader:" << info << '\n'
                << shader << std::endl;
   }

   return glShader;
}
std::string Shader::readFile(const std::string fileName)
{
   std::string fileContent;
   std::ifstream fileStream;
   std::stringstream s;
   fileStream.exceptions(std::ifstream::badbit | std::ifstream::failbit);
   try
   {
      fileStream.open(fileName);
      s << fileStream.rdbuf();
      fileStream.close();
      fileContent = s.str();
      return fileContent;
   }
   catch (const std::istream::failure &e)
   {
      std::cerr << e.what() << '\n';
      return "";
   }
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