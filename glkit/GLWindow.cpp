#include "GLWindow.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>

glkit::GLWindow::GLWindow(std::string windowTitle,
                          int windowWidth,
                          int windowHeight) : mWindowTitle(windowTitle),
                                              mWindowWidth(windowWidth),
                                              mWindowHeight(windowHeight)
{
    createWindow();
    enterLoop();
}

bool glkit::GLWindow::createWindow()
{
    if (!glfwInit())
    {
        std::cerr << ">>> createWindow() failed" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    mWindow = glfwCreateWindow(mWindowWidth, mWindowHeight, mWindowTitle.c_str(), NULL, NULL);

    if (mWindow == NULL)
    {
        std::cerr << "glfwCreateWindow() failed!" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(mWindow);
    onWindowCreated(mWindow);
}

void glkit::GLWindow::onWindowCreated(GLFWwindow *window)
{
    glfwSetFramebufferSizeCallback(window, onFrameBufferSize);
    glfwSetCursorPosCallback(window, onCursorPos);
    glfwSetScrollCallback(window, onScroll);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    onSetup();
}

void glkit::GLWindow::onSetup()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glEnable(GL_MULTISAMPLE);
}

void glkit::GLWindow::enterLoop()
{
    if (!mWindow)
    {
        return;
    }

    while (!glfwWindowShouldClose(mWindow))
    {
        onUpdate();
        onDraw();
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
}

glkit::GLWindow::~GLWindow()
{
    if (mWindow)
    {
        glfwDestroyWindow(mWindow);
        mWindow = nullptr;
    }
}