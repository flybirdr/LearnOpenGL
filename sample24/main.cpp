#define GLFW_INCLUDE_NONE
#define GLM_FORCE_MESSAGES
#include "camera.h"
#include "mesh.h"
#include "model.h"
#include "shader.h"
#include "stb_image.h"
#include "texture.h"
#include "vertex.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <fstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>

const int screenWidth = 1440;
const int screenHeight = 900;
float lastTime = 0;
bool firstMouse = true;
float lastX;
float lastY;

std::string cwd = CWD;

Camera camera(glm::vec3(0.0f, 3.0f, 4.0f));

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
  glfwWindowHint(GLFW_SAMPLES, 4);

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

  Shader shader(cwd + "/ver_1.vert", cwd + "/frag_1.frag");

  shader.use();
  shader.setFloat("material.shininess", 64.0f);

  shader.setVec3("viewPos", glm::value_ptr(camera.getPosition()));

  shader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
  shader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
  shader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
  shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

  shader.setVec3("dotLight.position", glm::value_ptr(camera.getPosition()));
  shader.setFloat("dotLight.constant", 1.0f);
  shader.setFloat("dotLight.linear", 0.09f);
  shader.setFloat("dotLight.quadratic", 0.032f);
  shader.setVec3("dotLight.ambient", 0.2f, 0.2f, 0.2f);
  shader.setVec3("dotLight.diffuse", 0.5f, 0.5f, 0.5f);
  shader.setVec3("dotLight.specular", 1.0f, 1.0f, 1.0f);

  shader.setVec3("spotLight.position", glm::value_ptr(camera.getPosition()));
  shader.setVec3("spotLight.direction", glm::value_ptr(camera.getPosition()));
  shader.setFloat("spotLight.innerCutoff", 0.2f);
  shader.setFloat("spotLight.outterCutoff", 0.3f);
  shader.setVec3("spotLight.ambient", 0.2f, 0.2f, 0.2f);
  shader.setVec3("spotLight.diffuse", 0.5f, 0.5f, 0.5f);
  shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);

  Model objModel(cwd + "/../nanosuit/nanosuit.obj");
  // Model objModel("/home/lovefantasy/Desktop/LearnOpenGL/sample17/91-21-iphonex/Iphone seceond version finished.obj");
  // Model objModel("/home/lovefantasy/Desktop/LearnOpenGL/sample17/97-free_091_aya_obj/091_W_Aya_30K.obj");
  // Model objModel("/home/lovefantasy/Desktop/LearnOpenGL/sample17/mountain/mount.blend1.obj");
  // Model objModel("/home/lovefantasy/Desktop/LearnOpenGL/sample17/AC Cobra/Shelby.obj");
  // Model objModel("/home/lovefantasy/Desktop/LearnOpenGL/sample17/xihu59td0bnk-AudiR8Spyder_2017/3D Models/Audi_R8_2017.obj");
  // Model objModel("/home/lovefantasy/Desktop/LearnOpenGL/sample17/8hd2hnno0ayo-aventador_sport/Avent_sport.obj");
  // Model objModel("/home/lovefantasy/Desktop/LearnOpenGL/sample17/93-pistol_tauros/pistol_Tauros/models/pistol_tauros.obj");

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  glEnable(GL_MULTISAMPLE);

  while (!glfwWindowShouldClose(window))
  {

    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    shader.use();

    glm::mat4 view = camera.getViewMatrix();
    shader.setMat4("view", glm::value_ptr(view));

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), screenWidth * 1.0f / screenHeight, 0.1f, 100.0f);
    shader.setMat4("projection", glm::value_ptr(projection));

    glm::mat4 model = glm::mat4(1.0f);
    float zoom = camera.getZoom();
    model = glm::scale(model, glm::vec3(zoom, zoom, zoom));
    model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    shader.setMat4("model", glm::value_ptr(model));
    shader.setVec2("singleColor", 0.0f, 0.0f);

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    objModel.draw(shader);
    glm::mat4 scaleModel = glm::scale(model, glm::vec3(1.005f, 1.005f, 1.005f));
    shader.setMat4("model", glm::value_ptr(scaleModel));
    shader.setVec2("singleColor", 1.0f, 0.0f);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    objModel.draw(shader);
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);

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