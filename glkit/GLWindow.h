#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <string>

namespace glkit
{
    class GLWindow
    {
    private:
        std::string mWindowTitle;
        int mWindowWidth, mWindowHeight;
        GLFWwindow *mWindow;

    private:
        static void onFrameBufferSize(GLFWwindow *window, int width, int height);
        static void onCursorPos(GLFWwindow *window, double xpos, double ypos);
        static void onScroll(GLFWwindow *window, double xoffset, double yoffset);

    protected:
        virtual bool createWindow();
        virtual void enterLoop();

        virtual void onWindowCreated(GLFWwindow *window);
        virtual void onSetup();
        virtual void onKey();
        virtual void onUpdate();
        virtual void onDraw();
    public:
        GLWindow() = delete;
        GLWindow(GLWindow &) = delete;
        GLWindow(GLWindow &&) = delete;
        GLWindow &operator=(GLWindow &) = delete;
        GLWindow &operator=(GLWindow &&) = delete;
        GLWindow(std::string windowTitle, int windowWidth, int windowHeight);

        virtual ~GLWindow();
    };

};

#endif // GLWINDOW_H
