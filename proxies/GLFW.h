//
// Created by Raphaël Dantzer on 13/10/16.
//

#ifndef PARTICLE_SYSTEM_GLFW_H
#define PARTICLE_SYSTEM_GLFW_H

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#include <utility>
#include <glm/vec3.hpp>
#include "ProxyInterface.h"
#include "../utils/FPSCamera.h"

namespace Proxy {

class GLFW : public ProxyInterface {
public:
    GLFW(std::pair<int, int> size, const char *title, std::pair<GLint, GLint> version);
    ~GLFW();

    GLFWwindow *getWindow() const;
    void        bindKeyCallback(GLFWkeyfun);
    void        bindCursorPosCallback(GLFWcursorposfun cursor);
    void        bindScrollCallback(GLFWscrollfun scroll);
    void        bindMouseCallback(GLFWmousebuttonfun button);

    const FPSCamera &getCamera() const;
    void setCamera(const FPSCamera &camera);

private:
    GLFWwindow              *_window;
    const char              *_title;
    std::pair<int, int>     _size;
    std::pair<int, int>     _framebuffer;
    FPSCamera               _camera;

    void PreInit() override;
    void Init() override;
    void PostInit() override;
};

}

#endif //PARTICLE_SYSTEM_GLFW_H
