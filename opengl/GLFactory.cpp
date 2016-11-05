//
// Created by Raphaël Dantzer on 05/11/16.
//

#include "GLFactory.h"

GLFactory::GLFactory() :
        _usage(GL_STATIC_DRAW),
        _vaoIdx(0),
        _vboIdx(0)
{}

GLFactory::~GLFactory() {
    for (auto it : _objects)
    {
        glDeleteVertexArrays(1, &it->_vao);
        glDeleteBuffers(1, &it->_vbo);
    }
}

Buffer *GLFactory::RegisterF(GLvoid *data, std::size_t size, GLint count, GLenum target) {
    Buffer *buffer = new Buffer;

    glGenVertexArrays(1, &buffer->_vao);
    glGenBuffers(1, &buffer->_vbo);

    glBindVertexArray(buffer->_vao);

    glBindBuffer(target, buffer->_vbo);
    glBufferData(target, size, data, _usage);

    glVertexAttribPointer(_vaoIdx, count, GL_FLOAT, GL_FALSE, count * sizeof(GLfloat), data);
    glEnableVertexAttribArray(_vaoIdx);

    glBindVertexArray(0);

    _objects.push_back(buffer);
    ++_vaoIdx;
    return buffer;
}

void GLFactory::setUsage(GLenum usage) {
    _usage = usage;
}
