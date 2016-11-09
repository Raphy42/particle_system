//
// Created by Raphaël Dantzer on 03/11/16.
//

#ifndef PARTICLE_SYSTEM_BUFFER_H
#define PARTICLE_SYSTEM_BUFFER_H

#include <OpenGL/gl3.h>

struct Buffer {
    GLuint                      _vao;
    std::vector<GLuint>         _vbo;
};

#endif //PARTICLE_SYSTEM_BUFFER_H
