//
// Created by Raphael DANTZER on 9/28/16.
//

#include <iostream>
#include "GLFW.h"

void GLFW::init() const
{
    if (!glfwInit())
        exit(EXIT_FAILURE);
}

void GLFW::post_init()
{
    glfwGetFramebufferSize(_window, &_framebuffer_width, &_framebuffer_height);
}

extern "C" {
    static void     error_callback(int errcode, const char *message)
    {
        std::cout << message << ": " << errcode << std::endl;
    }

    static void     key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        if (action == GLFW_PRESS)
            switch (key)
            {
                case GLFW_KEY_ESCAPE:
                    glfwSetWindowShouldClose(window, GL_TRUE);
                default:
                    break ;
            }
    }
}

/**
 * GLFW constructor
 * @param _width Window width
 * @param _height Window height
 * @param title
 * @return
 */
GLFW::GLFW(int width, int height, const char *title) :
        _window(nullptr), _width(width), _height(height), _title(title)
{
    this->init();
    glfwSetErrorCallback(error_callback);

//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    assert(_window = glfwCreateWindow(width, height, _title, nullptr, nullptr));
    glfwMakeContextCurrent(_window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(_window, key_callback);
    printf("%s: %s\n", glGetString(GL_RENDERER), glGetString(GL_VERSION));
    this->post_init();
}

int GLFW::should_exit()
{
    glfwPollEvents();
    return (!glfwWindowShouldClose(_window));
}

void GLFW::render() const
{
    glfwSwapBuffers(_window);
}

GLFW::~GLFW()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}

GLFW::GLFW() : _window(nullptr), _width(1200), _height(800), _title("Window")
{

}
