//
// Created by Raphaël Dantzer on 17/10/16.
//

#include <string>
#include "Program.h"
#include "../utils/FileLogger.h"

OpenGL::Program::Program(std::string vs, std::string fs) :
        _program(0), _vs(0), _fs(0) {
    _vs = build(vs, GL_VERTEX_SHADER);
    _fs = build(fs, GL_FRAGMENT_SHADER);
    _program = glCreateProgram();
    glAttachShader(_program, _vs);
    glAttachShader(_program, _fs);
    glLinkProgram(_program);

    int status = 0;

    glGetProgramiv(_program, GL_LINK_STATUS, &status);
    if (!status) {
        char buffer[512];
        glGetProgramInfoLog(_program, 512, NULL, buffer);
        FLOG_WARNING(buffer);
    }
}

OpenGL::Program::~Program() {
    if (_program)
        glDeleteProgram(_program);
    if (_vs)
        glDeleteShader(_vs);
    if (_fs)
        glDeleteShader(_fs);
}

void OpenGL::Program::bind() const {
    glUseProgram(_program);
}

GLuint OpenGL::Program::build(std::string &filename, GLenum shaderType) {
    GLuint shader;
    std::ifstream t(filename);
    std::string str;

    if (!t.is_open())
        FLOG_CRITICAL("Failed to open file");

    t.seekg(0, std::ios::end);
    str.reserve(static_cast<unsigned long>(t.tellg()));
    t.seekg(0, std::ios::beg);
    str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    const GLchar *buffer = str.c_str();
    GLint size = static_cast<GLint>(str.length());

    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &buffer, &size);
    glCompileShader(shader);

    int status = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        char log[512];
        glGetShaderInfoLog(shader, 512, NULL, log);
        FLOG_WARNING(log);
    }
    return (shader);
}

GLint OpenGL::Program::uniform(const char *uniform) const {
    return glGetUniformLocation(_program, uniform);
}

