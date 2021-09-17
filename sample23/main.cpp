#define GLFW_INCLUDE_NONE
#define GLM_FORCE_MESSAGES
#include "render/camera.h"
#include "render/mesh.h"
#include "render/model.h"
#include "render/shader.h"
#include "render/texture.h"
#include "render/vertex.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <fstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <stb/stb_image.h>
#include <unistd.h>

const int screenWidth = 1440;
const int screenHeight = 900;
float lastTime = 0;
bool firstMouse = true;
float lastX;
float lastY;
//绝对路径和相对路径同时出现std会异常
std::string pwd = CWD;

Camera camera(glm::vec3(1.0f, 5.0f, 8.0f));

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
GLuint loadCubeTexture(std::vector<std::string> &textures);

int main(int, char **)
{

  std::cout << "cwd:" << pwd << std::endl;
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "failed to create gl window!" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouseCallback);
  glfwSetScrollCallback(window, scrollCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  Shader modelShader(pwd + "/render/model.vs", pwd + "/render/model.fs");

  modelShader.use();
  modelShader.setFloat("material.shininess", 64.0f);

  modelShader.setVec3("viewPos", glm::value_ptr(camera.getPosition()));

  modelShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
  modelShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
  modelShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
  modelShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

  modelShader.setVec3("dotLight.position", glm::value_ptr(camera.getPosition()));
  modelShader.setFloat("dotLight.constant", 1.0f);
  modelShader.setFloat("dotLight.linear", 0.09f);
  modelShader.setFloat("dotLight.quadratic", 0.032f);
  modelShader.setVec3("dotLight.ambient", 0.2f, 0.2f, 0.2f);
  modelShader.setVec3("dotLight.diffuse", 0.5f, 0.5f, 0.5f);
  modelShader.setVec3("dotLight.specular", 1.0f, 1.0f, 1.0f);

  modelShader.setVec3("spotLight.position", glm::value_ptr(camera.getPosition()));
  modelShader.setVec3("spotLight.direction", glm::value_ptr(camera.getPosition()));
  modelShader.setFloat("spotLight.innerCutoff", 0.2f);
  modelShader.setFloat("spotLight.outterCutoff", 0.3f);
  modelShader.setVec3("spotLight.ambient", 0.2f, 0.2f, 0.2f);
  modelShader.setVec3("spotLight.diffuse", 0.5f, 0.5f, 0.5f);
  modelShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);

  Model modelObj("/home/lovefantasy/Desktop/LearnOpenGL/nanosuit/nanosuit.obj");

  std::vector<Vertex> cubeVerteices;
  std::vector<unsigned int> cubeIndeices;
  std::vector<Texture> cubeTextures;
  cubeVerteices.push_back(Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)});

  cubeVerteices.push_back(Vertex{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)});

  cubeVerteices.push_back(Vertex{glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)});

  cubeVerteices.push_back(Vertex{glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)});

  cubeVerteices.push_back(Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)});

  cubeVerteices.push_back(Vertex{glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
  cubeVerteices.push_back(Vertex{glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)});

  for (int i = 0; i < 36; i++)
  {
    cubeIndeices.push_back(i);
  }
  Texture cubeTextureDiffuse("texture_diffuse", "../container2.png");
  Texture cubeTextureSpecular("texture_specular", "../container2_specular.png");
  cubeTextures.push_back(cubeTextureDiffuse);
  cubeTextures.push_back(cubeTextureSpecular);
  Mesh cube(cubeVerteices, cubeIndeices, cubeTextures);

  Texture textureGrass("texture_diffuse", pwd + "/grass.png");
  Texture textureWindow("texture_diffuse", pwd + "/blending_transparent_window.png");
  std::vector<Vertex> faceVerteices;
  std::vector<unsigned int> faceIndeices;
  faceVerteices.push_back(Vertex{glm::vec3(-0.5f, -0.5f, -0.8f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)});
  faceVerteices.push_back(Vertex{glm::vec3(0.5f, -0.5f, -0.8f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)});
  faceVerteices.push_back(Vertex{glm::vec3(0.5f, 0.5f, -0.8f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)});
  faceVerteices.push_back(Vertex{glm::vec3(0.5f, 0.5f, -0.8f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)});
  faceVerteices.push_back(Vertex{glm::vec3(-0.5f, 0.5f, -0.8f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)});
  faceVerteices.push_back(Vertex{glm::vec3(-0.5f, -0.5f, -0.8f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)});
  for (int i = 0; i < 6; i++)
  {
    faceIndeices.push_back(i);
  }
  Shader faceShader(pwd + "/render/face.vs", pwd + "/render/face.fs");

  GLuint faceVao, faceVbo, faceEbo;
  glGenBuffers(1, &faceVbo);
  glGenBuffers(1, &faceEbo);

  glBindBuffer(GL_ARRAY_BUFFER, faceVbo);
  glBufferData(GL_ARRAY_BUFFER, faceVerteices.size() * sizeof(Vertex), &faceVerteices[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceEbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceIndeices.size() * sizeof(unsigned int), &faceIndeices[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glGenVertexArrays(1, &faceVao);
  glBindVertexArray(faceVao);
  glBindBuffer(GL_ARRAY_BUFFER, faceVbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));
  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceEbo);
  glBindVertexArray(0);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_STENCIL_TEST);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  // glEnable(GL_CULL_FACE);
  // glCullFace(GL_BACK);
  // glFrontFace(GL_CCW);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
  glBlendEquation(GL_ADD);

  GLuint fbo, fboTexture, rbo;
  glGenFramebuffers(1, &fbo);
  glGenTextures(1, &fboTexture);
  glGenBuffers(1, &rbo);

  glBindTexture(GL_TEXTURE_2D, fboTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  glBindTexture(GL_TEXTURE_2D, 0);

  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    std::cout << "error: framebuffer is not complete!" << std::endl;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  std::vector<Vertex> finalVerteices;
  std::vector<unsigned int> finalIndeices;
  finalVerteices.push_back(Vertex{glm::vec3(-1.0f, -1.0f, -0.8f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)});
  finalVerteices.push_back(Vertex{glm::vec3(1.0f, -1.0f, -0.8f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)});
  finalVerteices.push_back(Vertex{glm::vec3(1.0f, 1.0f, -0.8f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)});
  finalVerteices.push_back(Vertex{glm::vec3(1.0f, 1.0f, -0.8f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)});
  finalVerteices.push_back(Vertex{glm::vec3(-1.0f, 1.0f, -0.8f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)});
  finalVerteices.push_back(Vertex{glm::vec3(-1.0f, -1.0f, -0.8f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)});
  for (int i = 0; i < 6; i++)
  {
    finalIndeices.push_back(i);
  }
  Shader finalShader(pwd + "/render/texture.vs", pwd + "/render/texture.fs");

  GLuint finalVao, finalVbo, finalEbo;
  glGenBuffers(1, &finalVbo);
  glGenBuffers(1, &finalEbo);

  glBindBuffer(GL_ARRAY_BUFFER, finalVbo);
  glBufferData(GL_ARRAY_BUFFER, finalVerteices.size() * sizeof(Vertex), &finalVerteices[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, finalEbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, finalIndeices.size() * sizeof(unsigned int), &finalIndeices[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glGenVertexArrays(1, &finalVao);
  glBindVertexArray(finalVao);
  glBindBuffer(GL_ARRAY_BUFFER, finalVbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));
  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, finalEbo);
  glBindVertexArray(0);

  std::vector<std::string> skyBoxTextures{
      pwd + "/skybox/right.jpg",
      pwd + "/skybox/left.jpg",
      pwd + "/skybox/top.jpg",
      pwd + "/skybox/bottom.jpg",
      pwd + "/skybox/front.jpg",
      pwd + "/skybox/back.jpg"};

  GLuint __attribute__((unused)) skyboxTexture = loadCubeTexture(skyBoxTextures);

  Shader skyboxShader(pwd + "/render/skybox.vs", pwd + "/render/skybox.fs");
  skyboxShader.setInt("uSkybox", 0);
  float skyboxVertices[] = {
      // positions
      -1.0f, 1.0f, -1.0f,
      -1.0f, -1.0f, -1.0f,
      1.0f, -1.0f, -1.0f,
      1.0f, -1.0f, -1.0f,
      1.0f, 1.0f, -1.0f,
      -1.0f, 1.0f, -1.0f,

      -1.0f, -1.0f, 1.0f,
      -1.0f, -1.0f, -1.0f,
      -1.0f, 1.0f, -1.0f,
      -1.0f, 1.0f, -1.0f,
      -1.0f, 1.0f, 1.0f,
      -1.0f, -1.0f, 1.0f,

      1.0f, -1.0f, -1.0f,
      1.0f, -1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,
      1.0f, 1.0f, -1.0f,
      1.0f, -1.0f, -1.0f,

      -1.0f, -1.0f, 1.0f,
      -1.0f, 1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,
      1.0f, -1.0f, 1.0f,
      -1.0f, -1.0f, 1.0f,

      -1.0f, 1.0f, -1.0f,
      1.0f, 1.0f, -1.0f,
      1.0f, 1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,
      -1.0f, 1.0f, 1.0f,
      -1.0f, 1.0f, -1.0f,

      -1.0f, -1.0f, -1.0f,
      -1.0f, -1.0f, 1.0f,
      1.0f, -1.0f, -1.0f,
      1.0f, -1.0f, -1.0f,
      -1.0f, -1.0f, 1.0f,
      1.0f, -1.0f, 1.0f};

  GLuint skyboxVao, skyboxVbo;
  glGenBuffers(1, &skyboxVbo);
  glBindBuffer(GL_ARRAY_BUFFER, skyboxVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenVertexArrays(1, &skyboxVao);
  glBindVertexArray(skyboxVao);
  glBindBuffer(GL_ARRAY_BUFFER, skyboxVbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  GLuint uboMatrices;
  glGenBuffers(1, &uboMatrices);
  glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, nullptr, GL_STATIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  GLuint uMatriesIndex = glGetUniformBlockIndex(faceShader.program(), "Matrices");
  glUniformBlockBinding(faceShader.program(), uMatriesIndex, 2);
  uMatriesIndex = glGetUniformBlockIndex(modelShader.program(), "Matrices");
  glUniformBlockBinding(modelShader.program(), uMatriesIndex, 2);
  uMatriesIndex = glGetUniformBlockIndex(skyboxShader.program(), "Matrices");
  glUniformBlockBinding(skyboxShader.program(), uMatriesIndex, 2);

  glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboMatrices);
  // glBindBufferRange(GL_UNIFORM_BUFFER, 2, uboMatrices, 0, sizeof(glm::mat4) * 2);

  glm::mat4 projection = glm::perspective(glm::radians(45.0f), screenWidth * 1.0f / screenHeight, 0.1f, 100.0f);

  glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  Shader modelNormalShader(pwd + "/render/normal.vs", pwd + "/render/normal.gs", pwd + "/render/normal.fs");
  
  Shader houseShader(pwd + "/render/house.vs", pwd + "/render/house.gs", pwd + "/render/house.fs");

  uMatriesIndex = glGetUniformBlockIndex(houseShader.program(), "Matrices");
  glUniformBlockBinding(houseShader.program(), uMatriesIndex, 2);
  GLfloat houseVertices[] = {
      -0.5f, -0.5f, 0.3f};
  GLfloat houseColor[] = {
      0.5f, 0.5f, 0.0f
      };

  while (!glfwWindowShouldClose(window))
  {

    processInput(window);

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    float zoom = camera.getZoom();
    model = glm::scale(model, glm::vec3(zoom, zoom, zoom));
    model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, screenWidth, screenHeight);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    modelShader.use();
    modelShader.setMat4("model", glm::value_ptr(model));
    modelShader.setVec2("singleColor", 0.0f, 0.0f);
    modelShader.setFloat("time", glfwGetTime());

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    modelObj.draw(modelShader);

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    glm::mat4 scaleModel = glm::scale(model, glm::vec3(1.005f, 1.005f, 1.005f));
    modelShader.setMat4("model", glm::value_ptr(scaleModel));
    modelShader.setVec2("singleColor", 1.0f, 0.0f);
    modelObj.draw(modelShader);
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_GREATER);

    modelShader.setMat4("model", glm::value_ptr(model));
    modelShader.setVec2("singleColor", 0.0f, 0.0f);
    modelObj.draw(modelShader);

    modelShader.setMat4("model", glm::value_ptr(model));
    cube.draw(modelShader);

    modelObj.draw(modelNormalShader);
    cube.draw(modelNormalShader);

    faceShader.use();

    faceShader.setInt("shader1", 4);
    glActiveTexture(GL_TEXTURE0 + 4);
    glBindTexture(GL_TEXTURE_2D, textureGrass.getTextureId());
    glBindVertexArray(faceVao);
    glm::mat4 faceModel = glm::translate(model, glm::vec3(0.0f, 5.0f, 3.0f));
    faceShader.setMat4("model", glm::value_ptr(faceModel));
    glDrawElements(GL_TRIANGLES, faceIndeices.size(), GL_UNSIGNED_INT, 0);

    faceModel = glm::translate(model, glm::vec3(0.5f, 5.0f, 2.0f));
    faceShader.setMat4("model", glm::value_ptr(faceModel));
    glDrawElements(GL_TRIANGLES, faceIndeices.size(), GL_UNSIGNED_INT, 0);

    glActiveTexture(GL_TEXTURE0 + 4);
    glBindTexture(GL_TEXTURE_2D, textureWindow.getTextureId());
    glBindVertexArray(faceVao);
    faceModel = glm::translate(model, glm::vec3(1.8f, 5.0f, 4.0f));
    faceShader.setMat4("model", glm::value_ptr(faceModel));
    glDrawElements(GL_TRIANGLES, faceVerteices.size(), GL_UNSIGNED_INT, 0);
    faceModel = glm::translate(model, glm::vec3(1.3f, 5.0f, 6.0f));
    faceShader.setMat4("model", glm::value_ptr(faceModel));
    glDrawElements(GL_TRIANGLES, faceVerteices.size(), GL_UNSIGNED_INT, 0);

    houseShader.use();
    glVertexAttribPointer(0, sizeof(houseVertices), GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), houseVertices);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, sizeof(houseColor), GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), houseColor);
    glEnableVertexAttribArray(1);
    glDrawArrays(GL_POINTS, 0, 1);

    // // glDepthMask(GL_FALSE);
    // glDepthFunc(GL_LEQUAL);
    // skyboxShader.use();
    // glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
    // glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    // glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(skyboxView));
    // glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    // glBindVertexArray(skyboxVao);
    // glDrawArrays(GL_TRIANGLES, 0, 36);
    // glBindVertexArray(0);
    // glDepthFunc(GL_LESS);
    // // glDepthMask(GL_TRUE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClearColor(0.5f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glViewport(0, 0, screenWidth, screenHeight);
    finalShader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fboTexture);
    glBindVertexArray(finalVao);
    glDrawElements(GL_TRIANGLES, finalIndeices.size(), GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window)
{
  float deltaTime = glfwGetTime() - lastTime;
  lastTime = glfwGetTime();

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }
  else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    camera.processKeyBoard(FORWARD, deltaTime);
  }
  else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    camera.processKeyBoard(BACKWARD, deltaTime);
  }
  else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    camera.processKeyBoard(LEFT, deltaTime);
  }
  else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    camera.processKeyBoard(RIGHT, deltaTime);
  }
}
void mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
    return;
  }
  camera.processMouseMovement(xpos - lastX, ypos - lastY, true);
  lastX = xpos;
  lastY = ypos;
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
  camera.processMouseScroll(yoffset);
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

GLuint loadCubeTexture(std::vector<std::string> &textures)
{
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  for (size_t i = 0; i < textures.size(); i++)
  {
    const std::string &path = textures[i];
    int width, height, channels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (data)
    {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    }
    else
    {
      std::cerr << "failed to load " << path << std::endl;
    }
  }

  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  return texture;
}