#ifndef MESH_H
#define MESH_H

#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"
#include <glad/glad.h>
#include <vector>

namespace glkit
{
   class Mesh
   {
   private:
      std::vector<Vertex> mVertecis;
      std::vector<unsigned int> mIndeices;
      std::vector<Texture> mTextures;
      GLuint mVao, mVbo, mEbo;
      mutable int mUseCount;
      void incRef() const;
      void release();

   public:
      Mesh() = delete;
      Mesh(std::vector<Vertex> verteices,
           std::vector<unsigned int> indeices,
           std::vector<Texture> textures);

      Mesh(const Mesh &src);

      Mesh(const Mesh &&src);

      Mesh &operator=(const Mesh &src);

      Mesh &operator=(const Mesh &&src);

      void setupMesh();

      void draw(Shader &shader);

      GLuint getVao();

      GLuint getEbo();

      std::vector<unsigned int> &getIndices();

      std::vector<Texture> &getTextures();

      ~Mesh();
   };
};
#endif