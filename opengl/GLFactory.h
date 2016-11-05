//
// Created by Raphaël Dantzer on 05/11/16.
//

#ifndef PARTICLE_SYSTEM_GLFACTORY_H
#define PARTICLE_SYSTEM_GLFACTORY_H

#include <cstddef>
#include <utility>
#include <vector>
#include "Buffer.h"

class GLFactory {
public:
    GLFactory();
    ~GLFactory();

    void setUsage(GLenum usage);

    Buffer *RegisterF(void *data, size_t size, GLint count, GLenum target);

private:
    GLuint                  _vaoIdx;
    GLuint                  _vboIdx;
    GLenum                  _usage;
    std::vector<Buffer *>   _objects;
};


#endif //PARTICLE_SYSTEM_GLFACTORY_H
