#ifndef TEXTURE_H_H
#define TEXTURE_H_H

#include "stb_image.h"
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <string>

class Texture
{
private:
   GLuint mTextureId;

public:
   Texture(const std::string &name);
   GLuint getTextureId()
   {
      return mTextureId;
   }
   ~Texture();
};

Texture::Texture(const std::string &name)
{
   glGenTextures(1, &mTextureId);
   glBindTexture(GL_TEXTURE_2D, mTextureId);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   int width, height, channels;
   unsigned char *data = stbi_load(name.c_str(), &width, &height, &channels, 0);
   if (data)
   {
      GLenum format;
      if (channels == 1)
      {
         format = GL_RED;
      }
      else if (channels == 3)
      {
         format = GL_RGB;
      }
      else if (channels == 4)
      {
         format = GL_RGBA;
      }

      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
   }
   else
   {
      std::cout << "load " << name << " failed!" << std::endl;
   }

   glBindTexture(GL_TEXTURE_2D, 0);

   if (data)
   {
      stbi_image_free(data);
   }
}

Texture::~Texture()
{
   if (mTextureId)
   {
      glDeleteTextures(1, &mTextureId);
   }
}

#endif