#ifndef TEXTURE_H_H
#define TEXTURE_H_H

#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <stb_image.h>
#include <string>

namespace glkit
{
   class Texture
   {
   private:
      GLuint mTextureId;
      std::string type;
      std::string path;
      mutable int mUseCount;

      void incRef() const;
      void release();

   public:
      Texture() = delete;
      Texture(const std::string &type, const std::string &path);
      Texture(const Texture &src);
      Texture(const Texture &&src);
      Texture &operator=(const Texture &src);
      Texture &operator=(const Texture &&src);
      GLuint getTextureId();
      std::string &getType();
      std::string &getPath();
      std::string getName();
      ~Texture();
   };

};

#endif