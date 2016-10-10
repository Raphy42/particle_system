//
// Created by Raphaël Dantzer on 07/10/16.
//

#ifndef PARTICLE_SYSTEM_CLSHAREDCONTEXT_H
#define PARTICLE_SYSTEM_CLSHAREDCONTEXT_H

#include <OpenCL/opencl.h>
#include <string.h>
#include <iostream>
#include <OpenGL/OpenGL.h>
#include "utils.h"

namespace OpenCL {
    class SharedContext {
    public:
        SharedContext();
        ~SharedContext();

        cl_context getContext() const;
        cl_command_queue getQueue() const;

        void            BindKernel(std::string filename);

    private:
        void Init();

    private:
        cl_int                      _clStatus;
        cl_platform_id              *_platforms;
        cl_uint                     _numPlatforms;
        cl_device_id                *_deviceList;
        cl_uint                     _numDevices;
        cl_context                  _context;
        cl_command_queue            _queue;
        cl_program                  _program;
    };
}


#endif //PARTICLE_SYSTEM_CLSHAREDCONTEXT_H
