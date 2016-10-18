//
// Created by Raphaël Dantzer on 17/10/16.
//

#ifndef PARTICLE_SYSTEM_PROGRAM_H
#define PARTICLE_SYSTEM_PROGRAM_H

#include "../proxies/GLFW.h"

namespace OpenGL {
    class Program {
    public:
        Program(std::string vs, std::string fs);
        ~Program();

        void    bind() const;
        GLint   uniform(const char *uniform) const;

    private:
        GLuint      _program;
        GLuint      _vs;
        GLuint      _fs;

        GLuint build(std::string &filename, GLenum shaderType);
    };
}

#endif //PARTICLE_SYSTEM_PROGRAM_H
