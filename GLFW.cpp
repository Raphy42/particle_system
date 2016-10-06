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

static void error_callback(int errcode, const char *message)
{
    std::cout << message << ": " << errcode << std::endl;
}

GLFW::GLFW(int _width, int _height, const char *title) : _window(nullptr), _width(_width), _height(_height)
{
    this->init();
    glfwSetErrorCallback(error_callback);
    assert(_window = glfwCreateWindow(_width, _height, title, nullptr, nullptr));
    glfwMakeContextCurrent(_window);
    printf("%s: %s\n", glGetString(GL_RENDERER), glGetString(GL_VERSION));
}

void GLFW::render() const
{
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(_window);
}