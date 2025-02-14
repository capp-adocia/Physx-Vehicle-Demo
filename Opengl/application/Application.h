#pragma once
#include "../glframework/core.h"
#include <iostream>
#include <memory>
#define glApp App::getInstance()

class App
{
    using ResizeCallBack = void(*)(int width, int height);
    using KeyCallBack = void(*)(int key, int scancode, int action, int mods);
    using MouseCallBack = void(*)(int button, int action, int mods);
    using CursorCallBack = void(*)(double xpos, double ypos);
    using ScrollCallBack = void(*)(double offset);
private:
    App() {}
    int _width{ 0 };
    int _height{ 0 };
    GLFWwindow* _window{ nullptr };
    ResizeCallBack _ResizeCallBack{ nullptr }; // 窗口大小改变回调函数指针
    KeyCallBack _KeyCallBack{ nullptr }; // 键盘按键回调函数指针
    MouseCallBack _MouseCallBack{ nullptr }; // 鼠标按键回调函数指针
    CursorCallBack _CursorCallBack{ nullptr }; // 鼠标移动回调函数指针
    ScrollCallBack _ScrollCallBack{ nullptr }; // 鼠标滚轮回调函数指针

public:
    // 删除拷贝构造函数和赋值操作符，防止被复制
    App(const App&) = delete;
    App& operator=(const App&) = delete;

    // 静态方法来获取单例对象
    static App& getInstance() {
        static App instance;
        return instance;
    }
    inline int getWidth() const { return _width; }
    inline int getHeight() const { return _height; }
    inline GLFWwindow* getWindow() const { return _window; }
    inline void getCursorPosition(double* x, double* y) const { glfwGetCursorPos(getWindow(), x, y); }
    void init(const int& width = 800, const int& height = 600, const char* title = "OpenGL App")
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        _width = width;
        _height = height;
        _window = glfwCreateWindow(width, height, title, NULL, NULL);
        if (getWindow() == NULL) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            assert(false);
        }
        glfwMakeContextCurrent(getWindow());
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            assert(false);
        }
        glfwSetFramebufferSizeCallback(getWindow(), frameBufferSizeCallBack); // 设置窗口大小改变回调函数
        glfwSetKeyCallback(getWindow(), keyCallBack); // 设置按键回调函数
        glfwSetMouseButtonCallback(getWindow(), mouseCallBack); // 设置鼠标按键回调函数
        glfwSetCursorPosCallback(getWindow(), cursorCallBack); // 设置鼠标移动回调函数
        glfwSetScrollCallback(getWindow(), scrollCallBack); // 设置鼠标滚轮回调函数
    }
    bool update()
    {
        if (glfwWindowShouldClose(getWindow()))
        {
            return false;
        }

        glfwSwapBuffers(getWindow());
        glfwPollEvents();
        return true;
    }
    void destroy()
    {
        glfwDestroyWindow(_window);
        glfwTerminate();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    inline void setResizeCallBack(ResizeCallBack callback) { _ResizeCallBack = callback; } // 设置窗口大小改变回调函数
    inline void setKeyCallBack(KeyCallBack callback) { _KeyCallBack = callback; } // 设置按键回调函数
    inline void setMouseCallBack(MouseCallBack callback) { _MouseCallBack = callback; } // 设置鼠标按键回调函数
    inline void setCursorCallBack(CursorCallBack callback) { _CursorCallBack = callback; } // 设置渲染回调函数
    inline void setScrollCallBack(ScrollCallBack callback) { _ScrollCallBack = callback; } // 设置鼠标滚轮回调函数

    static void frameBufferSizeCallBack(GLFWwindow* window, int width, int height)
    {
        assert(glApp._ResizeCallBack);
        glViewport(0, 0, width, height);
        glApp._ResizeCallBack(width, height);
    }
    static void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        assert(glApp._KeyCallBack);
        glApp._KeyCallBack(key, scancode, action, mods);
    }
    static void mouseCallBack(GLFWwindow* window, int button, int action, int mods)
    {
        assert(glApp._MouseCallBack);
        glApp._MouseCallBack(button, action, mods);
    }
    static void cursorCallBack(GLFWwindow* window, double xpos, double ypos)
    {
        assert(glApp._CursorCallBack);
        glApp._CursorCallBack(xpos, ypos);
    }
    static void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset)
    {
        assert(glApp._ScrollCallBack);
        glApp._ScrollCallBack(yoffset);
    }
};
