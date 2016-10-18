//
// Created by Raphaël Dantzer on 13/10/16.
//

#ifndef PARTICLE_SYSTEM_GLFW_H
#define PARTICLE_SYSTEM_GLFW_H

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#include <utility>
#include "ProxyInterface.h"

namespace Proxy {

class GLFW : public ProxyInterface {
public:
    GLFW(std::pair<int, int> size, const char *title, std::pair<GLint, GLint> version);
    ~GLFW();

    GLFWwindow *getWindow() const;

private:
    GLFWwindow              *_window;
    const char              *_title;
    std::pair<int, int>     _size;
    std::pair<int, int>     _framebuffer;

    virtual void PreInit() override;
    virtual void Init() override;
    virtual void PostInit() override;

};

}

#endif //PARTICLE_SYSTEM_GLFW_H
