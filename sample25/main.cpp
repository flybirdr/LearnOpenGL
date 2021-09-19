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

  Shader planetShader(cwd + "/planet.vs", cwd + "/planet.fs");

  planetShader.use();
  planetShader.setFloat("material.shininess", 64.0f);

  planetShader.setVec3("viewPos", glm::value_ptr(camera.getPosition()));

  planetShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
  planetShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
  planetShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
  planetShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

  planetShader.setVec3("dotLight.position", glm::value_ptr(camera.getPosition()));
  planetShader.setFloat("dotLight.constant", 1.0f);
  planetShader.setFloat("dotLight.linear", 0.09f);
  planetShader.setFloat("dotLight.quadratic", 0.032f);
  planetShader.setVec3("dotLight.ambient", 0.2f, 0.2f, 0.2f);
  planetShader.setVec3("dotLight.diffuse", 0.5f, 0.5f, 0.5f);
  planetShader.setVec3("dotLight.specular", 1.0f, 1.0f, 1.0f);

  planetShader.setVec3("spotLight.position", glm::value_ptr(camera.getPosition()));
  planetShader.setVec3("spotLight.direction", glm::value_ptr(camera.getPosition()));
  planetShader.setFloat("spotLight.innerCutoff", 0.2f);
  planetShader.setFloat("spotLight.outterCutoff", 0.3f);
  planetShader.setVec3("spotLight.ambient", 0.2f, 0.2f, 0.2f);
  planetShader.setVec3("spotLight.diffuse", 0.5f, 0.5f, 0.5f);
  planetShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);

  Model planetModel(cwd + "/planet/planet.obj");

  Model rockModel(cwd + "/rock/rock.obj");
  Shader rockShader(cwd + "/rock.vs", cwd + "/rock.fs");

  unsigned int amount = 10000;
  glm::mat4 *modelMatrices = new glm::mat4[amount];
  srand(glfwGetTime()); // initialize random seed
  float radius = 40.0;
  float offset = 20.0f;
  for (unsigned int i = 0; i < amount; i++)
  {
    glm::mat4 model = glm::mat4(1.0f);
    // 1. translation: displace along circle with 'radius' in range [-offset, offset]
    float angle = (float)i / (float)amount * 360.0f;
    float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    float x = sin(angle) * radius + displacement;
    displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
    displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    float z = cos(angle) * radius + displacement;
    model = glm::translate(model, glm::vec3(x, y, z));

    // 2. scale: Scale between 0.05 and 0.25f
    float scale = (rand() % 20) / 100.0f + 0.05;
    model = glm::scale(model, glm::vec3(scale));

    // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
    float rotAngle = (rand() % 360);
    model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

    // 4. now add to list of matrices
    modelMatrices[i] = model;
  }

  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // GLuint rockVao;
  // glGenVertexArrays(1, &rockVao);

  std::vector<Mesh> &meshes = rockModel.getMeshes();
  for (size_t i = 0; i <meshes.size(); i++)
  {
    Mesh &mesh = meshes[i];
    GLuint vao = mesh.getVao();
    glBindVertexArray(vao);
    GLsizei mat4Size = sizeof(glm::mat4);
    GLsizei vec4Size = mat4Size/4;

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, mat4Size, 0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, mat4Size, (void *)vec4Size);
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, mat4Size, (void *)(2 * vec4Size));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, mat4Size, (void *)(3 * vec4Size));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
  }

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  glm::mat4 projection = glm::perspective(glm::radians(45.0f), screenWidth * 1.0f / screenHeight, 0.1f, 100.0f);

  while (!glfwWindowShouldClose(window))
  {

    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glm::mat4 view = camera.getViewMatrix();

    planetShader.use();
    planetShader.setMat4("projection", glm::value_ptr(projection));
    planetShader.setMat4("view", glm::value_ptr(view));


    glm::mat4 model = glm::mat4(1.0f);
    float zoom = camera.getZoom();
    model = glm::scale(model, glm::vec3(zoom, zoom, zoom));
    model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    planetShader.setMat4("model", glm::value_ptr(model));
    planetModel.draw(planetShader);

    rockShader.use();
    rockShader.setMat4("projection", glm::value_ptr(projection));
    rockShader.setMat4("view", glm::value_ptr(view));

    // rockModel.draw(rockShader);

    rockShader.setInt("material.texture_diffuse1", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, meshes[0].getTextures()[0].getTextureId());
    for (size_t i = 0; i < meshes.size(); i++)
    {
      GLuint vao = meshes[i].getVao();
      glBindVertexArray(vao);
      // glBindVertexArray(rockVao);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i].getEbo());
      std::vector<unsigned int> &indcies = meshes[i].getIndices();
      glDrawElementsInstanced(GL_TRIANGLES, indcies.size(), GL_UNSIGNED_INT, 0,amount);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
    }

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