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

const int screenWidth = 1440;
const int screenHeight = 900;
float lastTime = 0;
bool firstMouse = true;
float lastX;
float lastY;
// 绝对路径和相对路径同时出现std会异常
std::string pwd = "/home/lovefantasy/Desktop/LearnOpenGL/sample20";

Camera camera(glm::vec3(1.0f, 5.0f, 8.0f));

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

int main(int, char **)
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
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

  GLuint vao, vbo, ebo;
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, faceVerteices.size() * sizeof(Vertex), &faceVerteices[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceIndeices.size() * sizeof(unsigned int), &faceIndeices[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));
  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBindVertexArray(0);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_STENCIL_TEST);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
  glBlendEquation(GL_ADD);

  GLuint fbo, fboTexture, rbo;
  glGenFramebuffers(1, &fbo);
  glGenTextures(1, &fboTexture);
  glGenBuffers(1, &rbo);

  glBindTexture(GL_TEXTURE_2D, fboTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
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

  while (!glfwWindowShouldClose(window))
  {

    processInput(window);

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), screenWidth * 1.0f / screenHeight, 0.1f, 100.0f);
    glm::mat4 model = glm::mat4(1.0f);
    float zoom = camera.getZoom();
    model = glm::scale(model, glm::vec3(zoom, zoom, zoom));
    model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, screenWidth, screenHeight);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    modelShader.use();
    modelShader.setMat4("view", glm::value_ptr(view));
    modelShader.setMat4("projection", glm::value_ptr(projection));
    modelShader.setMat4("model", glm::value_ptr(model));
    modelShader.setVec2("singleColor", 0.0f, 0.0f);

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
    modelShader.setMat4("model", glm::value_ptr(model));
    modelShader.setVec2("singleColor", 0.0f, 0.0f);
    modelObj.draw(modelShader);

    modelShader.setMat4("view", glm::value_ptr(view));
    modelShader.setMat4("projection", glm::value_ptr(projection));
    modelShader.setMat4("model", glm::value_ptr(model));
    cube.draw(modelShader);

    faceShader.use();

    faceShader.setMat4("view", glm::value_ptr(view));
    faceShader.setMat4("projection", glm::value_ptr(projection));
    faceShader.setInt("shader1", 4);
    glActiveTexture(GL_TEXTURE0 + 4);
    glBindTexture(GL_TEXTURE_2D, textureGrass.getTextureId());
    glBindVertexArray(vao);
    glm::mat4 faceModel = glm::translate(model, glm::vec3(0.0f, 5.0f, 3.0f));
    faceShader.setMat4("model", glm::value_ptr(faceModel));
    glDrawElements(GL_TRIANGLES, faceIndeices.size(), GL_UNSIGNED_INT, 0);

    faceModel = glm::translate(model, glm::vec3(0.5f, 5.0f, 2.0f));
    faceShader.setMat4("model", glm::value_ptr(faceModel));
    glDrawElements(GL_TRIANGLES, faceIndeices.size(), GL_UNSIGNED_INT, 0);

    glActiveTexture(GL_TEXTURE0 + 4);
    glBindTexture(GL_TEXTURE_2D, textureWindow.getTextureId());
    glBindVertexArray(vao);
    faceModel = glm::translate(model, glm::vec3(1.8f, 5.0f, 4.0f));
    faceShader.setMat4("model", glm::value_ptr(faceModel));
    glDrawElements(GL_TRIANGLES, faceVerteices.size(), GL_UNSIGNED_INT, 0);
    faceModel = glm::translate(model, glm::vec3(1.3f, 5.0f, 6.0f));
    faceShader.setMat4("model", glm::value_ptr(faceModel));
    glDrawElements(GL_TRIANGLES, faceVerteices.size(), GL_UNSIGNED_INT, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClearColor(0.5f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glViewport(0, 0, screenWidth, screenHeight);
    finalShader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fboTexture);
    glBindVertexArray(finalVao);
    glDrawElements(GL_TRIANGLES, finalIndeices.size(), GL_UNSIGNED_INT, 0);

    // glViewport(0, 0, screenWidth/2, screenHeight/2);
    // finalShader.use();
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, fboTexture);
    // glBindVertexArray(finalVao);
    // glDrawElements(GL_TRIANGLES, finalIndeices.size(), GL_UNSIGNED_INT, 0);

    // glViewport(screenWidth / 2, 0, screenWidth / 2, screenHeight / 2);
    // finalShader.use();
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, fboTexture);
    // glBindVertexArray(finalVao);
    // glDrawElements(GL_TRIANGLES, finalIndeices.size(), GL_UNSIGNED_INT, 0);

    // glViewport(0, screenHeight / 2, screenWidth / 2, screenHeight / 2);
    // finalShader.use();
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, fboTexture);
    // glBindVertexArray(finalVao);
    // glDrawElements(GL_TRIANGLES, finalIndeices.size(), GL_UNSIGNED_INT, 0);

    // glViewport(screenWidth / 2, screenHeight / 2, screenWidth / 2, screenHeight / 2);
    // finalShader.use();
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, fboTexture);
    // glBindVertexArray(finalVao);
    // glDrawElements(GL_TRIANGLES, finalIndeices.size(), GL_UNSIGNED_INT, 0);

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