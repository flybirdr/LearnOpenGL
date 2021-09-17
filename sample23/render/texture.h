#ifndef TEXTURE_H_H
#define TEXTURE_H_H

#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <stb/stb_image.h>
#include <string>

class Texture
{
private:
   GLuint mTextureId;
   std::string mType;
   std::string mPath;
   mutable int mUseCount;

   void incRef() const
   {
      mUseCount++;
   }
   void release()
   {
      if (--mUseCount == 0)
      {
         if (mTextureId)
         {
            glDeleteTextures(1, &mTextureId);
            mTextureId = 0;
         }
         mUseCount = 0;
      }
   }

public:
   Texture() = delete;
   Texture(const std::string &mType, const std::string &mPath);
   Texture(const Texture &src)
   {
      if (mTextureId == src.mTextureId)
      {
         return;
      }
      release();
      src.incRef();
      this->mTextureId = src.mTextureId;
      this->mType = src.mType;
      this->mPath = src.mPath;
      this->mUseCount = src.mUseCount;
   }
   Texture(const Texture &&src)
   {
      if (mTextureId == src.mTextureId)
      {
         return;
      }
      release();
      src.incRef();
      this->mTextureId = src.mTextureId;
      this->mType = src.mType;
      this->mPath = src.mPath;
      this->mUseCount = src.mUseCount;
   }

   Texture &operator=(const Texture &src)
   {
      if (mTextureId == src.mTextureId)
      {
         return *this;
      }
      release();
      src.incRef();
      this->mTextureId = src.mTextureId;
      this->mType = src.mType;
      this->mPath = src.mPath;
      this->mUseCount = src.mUseCount;
      return *this;
   }
   Texture &operator=(const Texture &&src)
   {
      if (mTextureId == src.mTextureId)
      {
         return *this;
      }
      release();
      src.incRef();
      this->mTextureId = src.mTextureId;
      this->mType = src.mType;
      this->mPath = src.mPath;
      this->mUseCount = src.mUseCount;
      return *this;
   }

   GLuint getTextureId()
   {
      return mTextureId;
   }
   std::string &getType()
   {
      return mType;
   }
   std::string &getPath()
   {
      return mPath;
   }
   std::string getName()
   {
      return mPath.substr(mPath.find_last_of('/'), mPath.length());
   }
   ~Texture();
};

Texture::Texture(const std::string &mType, const std::string &mPath)
    : mTextureId(0), mType(mType), mPath(mPath), mUseCount(1)
{
   glGenTextures(1, &mTextureId);
   glBindTexture(GL_TEXTURE_2D, mTextureId);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   int width, height, channels;
   unsigned char *data = stbi_load(mPath.c_str(), &width, &height, &channels, 0);
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
      std::cout << "load texture " << mPath << " failed!" << std::endl;
   }

   glBindTexture(GL_TEXTURE_2D, 0);

   if (data)
   {
      stbi_image_free(data);
   }
}

Texture::~Texture()
{
   release();
}

#endif