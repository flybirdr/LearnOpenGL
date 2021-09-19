#ifndef MODEL_H_H
#define MODEL_H_H

#include "mesh.h"
#include "shader.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>

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
   void draw(Shader &shader)
   {
      for (unsigned int i = 0; i < mMeshes.size(); i++)
      {
         mMeshes[i].draw(shader);
      }
   }
   ~Model();

   std::vector<Mesh>& getMeshes(){
      return mMeshes;
   }
};

Model::Model(const std::string &modelFileName)
{
   mDirectory = modelFileName.substr(0, modelFileName.find_last_of('/') + 1);
   loadModel(modelFileName);
}

void Model::loadModel(const std::string &modelFileName)
{
   Assimp::Importer importer;
   const aiScene *sence = importer.ReadFile(modelFileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
   if (!sence || sence->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !sence->mRootNode)
   {
      std::cout << "load model failed:" << importer.GetErrorString() << std::endl;
      return;
   }
   processNode(sence, sence->mRootNode);
}
void Model::processNode(const aiScene *sence, aiNode *node)
{
   for (unsigned int i = 0; i < node->mNumMeshes; i++)
   {
      aiMesh *mesh = sence->mMeshes[node->mMeshes[i]];
      mMeshes.push_back(processMesh(sence, mesh));
   }
   for (unsigned int i = 0; i < node->mNumChildren; i++)
   {
      aiNode *child = node->mChildren[i];
      processNode(sence, child);
   }
}

Mesh Model::processMesh(const aiScene *sence, aiMesh *mesh)
{
   std::vector<Vertex> vecteices;
   std::vector<unsigned int> indecies;
   std::vector<Texture> textures;

   for (unsigned int i = 0; i < mesh->mNumVertices; i++)
   {
      aiVector3D v = mesh->mVertices[i];
      aiVector3D n = mesh->mNormals[i];
      Vertex vertex;
      vertex.postion = glm::vec3(v.x, v.y, v.z);
      vertex.normal = glm::vec3(n.x, n.y, n.z);
      if (mesh->mTextureCoords[0])
      {
         vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
      }
      else
      {
         vertex.texCoords = glm::vec2(0, 0);
      }
      vecteices.push_back(vertex);
   }

   for (unsigned int i = 0; i < mesh->mNumFaces; i++)
   {
      aiFace face = mesh->mFaces[i];
      for (unsigned int i = 0; i < face.mNumIndices; i++)
      {
         indecies.push_back(face.mIndices[i]);
      }
   }
   if (mesh->mMaterialIndex > 0)
   {
      aiMaterial *material = sence->mMaterials[mesh->mMaterialIndex];
      std::vector<Texture> ts = loadTextures(material);
      textures.insert(textures.end(), ts.begin(), ts.end());
   }

   return Mesh(vecteices, indecies, textures);
}

std::vector<Texture> Model::loadTextures(const aiMaterial *material)
{
   std::vector<Texture> textures;
   aiString name;
   bool skip = false;
   int diffuseCount = material->GetTextureCount(aiTextureType_DIFFUSE);
   for (int i = 0; i < diffuseCount; i++)
   {
      material->GetTexture(aiTextureType_DIFFUSE, i, &name);
      for (int i = 0; i < textures.size(); i++)
      {
         Texture &texture = textures[i];
         std::string curName = texture.getName();
         if (strcmp(curName.c_str(), name.C_Str()) == 0)
         {
            textures.push_back(texture);
            skip = true;
            break;
         }
      }
      if (!skip)
      {
         Texture texture("texture_diffuse", mDirectory + name.C_Str());
         textures.push_back(texture);
      }
   }

   int specularCount = material->GetTextureCount(aiTextureType_SPECULAR);
   for (int i = 0; i < specularCount; i++)
   {
      material->GetTexture(aiTextureType_SPECULAR, i, &name);
      for (int i = 0; i < textures.size(); i++)
      {
         Texture &texture = textures[i];
         std::string curName = texture.getName();
         if (strcmp(curName.c_str(), name.C_Str()) == 0)
         {
            textures.push_back(texture);
            skip = true;
            break;
         }
      }
      if (!skip)
      {
         Texture texture("texture_specular", mDirectory + name.C_Str());
         textures.push_back(texture);
      }
   }
   return textures;
}
Model::~Model()
{
}

#endif