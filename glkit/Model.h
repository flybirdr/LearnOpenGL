#ifndef MODEL_H_H
#define MODEL_H_H

#include "Mesh.h"
#include "Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>

namespace glkit
{
   class Model
   {
   private:
      std::string mDirectory;
      std::vector<Mesh> mMeshes;

   private:
      void loadModel(const std::string &modelFileName);
      void processNode(const aiScene *sence, aiNode *node);
      Mesh processMesh(const aiScene *sence, aiMesh *mesh);
      std::vector<Texture> loadTextures(const aiMaterial *material);

   public:
      Model(const std::string &modelFileName);
      std::vector<Mesh>& getMeshes();
      void draw(Shader &shader);
      ~Model();
   };

};
#endif