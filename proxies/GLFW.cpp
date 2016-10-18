//
// Created by Raphaël Dantzer on 13/10/16.
//

#include <iostream>

//glew? what is it ?
#define GLFW_INCLUDE_GLCOREARB

#include "GLFW.h"
#include <OpenGL/gl3.h>

#include "../utils/FileLogger.h"

extern FileLogger logger;

namespace Proxy {

    static void error_callback(int err, const char *message) {
        FLOG_INFO("glfw3 error callback");
        FLOG_INFO(message);
    }

    GLFW::GLFW(std::pair<int, int> size, const char *title, std::pair<GLint, GLint> version) : _size(size),
                                                                                               _title(title) {

        glfwSetErrorCallback(&error_callback);

        PreInit();

        FLOG_INFO(title);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version.first);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version.second);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        Init();

        PostInit();
        FLOG_INFO(glGetString(GL_VERSION));
        FLOG_INFO(glGetString(GL_RENDERER));
    }

    void GLFW::PreInit() {
        if (!glfwInit())
            FLOG_CRITICAL("Unable to init glfw");
        FLOG_INFO("glfw init");
    }

    void GLFW::Init() {
        _window = glfwCreateWindow(_size.first, _size.second, _title, nullptr, nullptr);
        if (!_window)
            FLOG_CRITICAL("Unable to create window");
        FLOG_INFO("glfwCreateWindow success");
    }

    static GLFW *getProxy(GLFWwindow *w) {
        return (static_cast<GLFW *>(glfwGetWindowUserPointer(w)));
    }

    static void key_callback(GLFWwindow *w, int key, int scancode, int action, int mods) {
        GLFW *p = getProxy(w);
        if (action == GLFW_PRESS) {
            switch (key) {
                case GLFW_KEY_ESCAPE:
                    glfwSetWindowShouldClose(w, true);
                    FLOG_INFO("escape key pressed, window should close");
                default:
                    break;
            }
        }
    }

    void GLFW::PostInit() {
        glfwSetWindowUserPointer(_window, this);
        glfwMakeContextCurrent(_window);
        glfwGetFramebufferSize(_window, &_framebuffer.first, &_framebuffer.second);

        //No need for callback manager just static function here


        glfwSetKeyCallback(_window, key_callback);


        //END CALLBACKS

        glViewport(0, 0, _framebuffer.first, _framebuffer.second);
    }

    GLFWwindow *GLFW::getWindow() const {
        return _window;
    }

    GLFW::~GLFW() {
        glfwDestroyWindow(_window);
        glfwTerminate();
        FLOG_INFO("glfwTerminate");
    }


}
