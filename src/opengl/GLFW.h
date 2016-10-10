//
// Created by Raphael DANTZER on 9/28/16.
//

#ifndef PARTICLE_SYSTEM_GLFW_H
#define PARTICLE_SYSTEM_GLFW_H

//#define GLFW_INCLUDE_GLCOREARB

//#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>

#include <cassert>

class GLFW
{

public:
    GLFW();
    GLFW(const int width, const int height, const char *title);
    ~GLFW();

    void            render(void) const;
    int             should_exit();

protected:
    GLFWwindow      *_window;
    int             _width;
    int             _height;
    int             _framebuffer_width;
    int             _framebuffer_height;
    const char      *_title;

    void            init(void) const;
    void            post_init();
};


#endif //PARTICLE_SYSTEM_GLFW_H
