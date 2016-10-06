//
// Created by Raphael DANTZER on 9/28/16.
//

#ifndef PARTICLE_SYSTEM_GLFW_H
#define PARTICLE_SYSTEM_GLFW_H

#define GLFW_INCLUDE_GLCOREARB

#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>

#include <cassert>

class GLFW
{

public:
    GLFW(const int width, const int height, const char *title);
    void render(void) const;

protected:
    GLFWwindow      *_window;
    int             _width;
    int             _height;

    void            init(void) const;
};


#endif //PARTICLE_SYSTEM_GLFW_H
