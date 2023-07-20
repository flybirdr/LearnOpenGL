#ifndef SHADER_H
#define SHADER_H

#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <string>

namespace glkit
{
   class Shader
   {
   private:
      GLuint mProgram;
      mutable int mUseCount;
      void incRef() const;
      void release();

   public:
      Shader(const std::string &vertex_shader, const std::string &frag_shader);
      Shader(const Shader &src);
      Shader(const Shader &&src);
      Shader &operator=(const Shader &src);
      Shader &operator=(const Shader &&src);
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

};
#endif