//
// Created by Raphaël Dantzer on 09/10/16.
//

#ifndef PARTICLE_SYSTEM_CORE_H
#define PARTICLE_SYSTEM_CORE_H


#include <vector>
#include "opengl/GLFW.h"
#include "opencl/SharedContext.h"

class Core {
public:
    Core();
    virtual ~Core();

    virtual void    InitContext() = 0;
    virtual void    RenderLoop() = 0;

protected:
private:
    GLFW                    _gl_wrap_context;
    OpenCL::SharedContext   _cl_wrap_context;
};


#endif //PARTICLE_SYSTEM_CORE_H
