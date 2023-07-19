#define GLFW_INCLUDE_NONE
// #define GLM_FORCE_MESSAGES
#include "Character.h"
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
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <map>
#include <sstream>

const int screenWidth = 1440;
const int screenHeight = 900;
float lastTime = 0;
bool firstMouse = true;
float lastX;
float lastY;

std::string cwd = CWD;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
std::map<GLchar, Character> characters;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void renderText(Shader &shader, GLuint vao, GLuint vbo, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

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

  std::string font = cwd + "/msyh.ttf";
  FT_Library ft;
  if (FT_Init_FreeType(&ft))
  {
    std::cerr << "could not load freetype library and init!" << std::endl;
  }
  FT_Face face;
  if (FT_New_Face(ft, font.c_str(), 0, &face))
  {
    std::cerr << "could not load font!" << std::endl;
  }
  FT_Set_Pixel_Sizes(face, 0, 48);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  for (GLubyte c = 0; c < 128; c++)
  {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    {
      std::cerr << "failed to load" << (char)c << std::endl;
      continue;
    }
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                 face->glyph->bitmap.width,
                 face->glyph->bitmap.rows,
                 0, GL_RED, GL_UNSIGNED_BYTE,
                 face->glyph->bitmap.buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Character character = {texture,
                           glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                           glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                           face->glyph->advance.x};
    characters.insert(std::pair<GLchar, Character>(c, character));
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  GLuint freeTypeVao, freeTypeVbo;
  glGenVertexArrays(1, &freeTypeVao);
  glGenBuffers(1, &freeTypeVbo);
  glBindVertexArray(freeTypeVao);
  glBindBuffer(GL_ARRAY_BUFFER, freeTypeVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, 0, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  Shader freeTypeShader(cwd + "/freetype.vs", cwd + "/freetype.fs");

  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // warning 不是float也会有问题的啊
  glm::mat4 textProjection = glm::ortho(0.0f, static_cast<GLfloat>(screenWidth), 0.0f, static_cast<GLfloat>(screenHeight));

  while (!glfwWindowShouldClose(window))
  {

    processInput(window);

    glViewport(0, 0, screenWidth, screenHeight);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 view = camera.getViewMatrix();

    freeTypeShader.use();
    freeTypeShader.setMat4("projection", glm::value_ptr(textProjection));
    renderText(freeTypeShader, freeTypeVao, freeTypeVbo, "hello freetype", 200.0f, 200.0f, 1.0f, glm::vec3(0.5f, 0.1f, 0.3f));

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void renderText(Shader &shader, GLuint vao, GLuint vbo, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
  shader.use();
  shader.setVec3("textColor", glm::value_ptr(color));
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(vao);

  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++)
  {
    Character ch = characters[*c];
    GLfloat xpos = x + ch.bearing.x * scale;
    GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;
    GLfloat w = ch.size.x * scale;
    GLfloat h = ch.size.y * scale;
    GLfloat vertices[6][4] = {{xpos, ypos + h, 0.0, 0.0},
                              {xpos, ypos, 0.0, 1.0},
                              {xpos + w, ypos, 1.0, 1.0},

                              {xpos, ypos + h, 0.0, 0.0},
                              {xpos + w, ypos, 1.0, 1.0},
                              {xpos + w, ypos + h, 1.0, 0.0}};
    glBindTexture(GL_TEXTURE_2D, ch.textureId);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    x += (ch.advance >> 6) * scale;
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
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