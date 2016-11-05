//
// Created by Raphaël Dantzer on 13/10/16.
//

#include <iostream>

#include "GLFW.h"

#include "../utils/FileLogger.h"

extern FileLogger logger;

namespace Proxy {

    static void error_callback(int err, const char *message) {
        FLOG_INFO("glfw3 error callback");
        FLOG_INFO(message);
    }

    GLFW::GLFW(std::pair<int, int> size, const char *title, std::pair<GLint, GLint> version) :
            _size(size),
            _title(title),
            _eye(0, 0, 0)
    {

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

    void GLFW::PostInit() {
        glfwSetWindowUserPointer(_window, this);
        glfwMakeContextCurrent(_window);
        glfwGetFramebufferSize(_window, &_framebuffer.first, &_framebuffer.second);

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

    void GLFW::bindKeyCallback(GLFWkeyfun key) {
        glfwSetKeyCallback(_window, key);
    }

    void GLFW::bindCursorPosCallback(GLFWcursorposfun cursor) {
        glfwSetCursorPosCallback(_window, cursor);
    }

    const glm::vec3 &GLFW::getEye() const {
        return _eye;
    }

    void GLFW::setEye(const glm::vec3 &eye) {
        _eye = eye;
    }

}
