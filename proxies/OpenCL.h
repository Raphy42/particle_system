//
// Created by Raphaël Dantzer on 18/10/16.
//

#ifndef PARTICLE_SYSTEM_OPENCL_H
#define PARTICLE_SYSTEM_OPENCL_H

#include "ProxyInterface.h"
//todo platform agnostic opencl include
#include <OpenCL/cl_gl_ext.h>
#include <OpenGL/OpenGL.h>
#include <vector>

namespace Proxy {
    class OpenCL : public ProxyInterface {
    public:
        OpenCL();
        ~OpenCL();

        void CreateKernel(const char *filename, const char *kernel_name);
        cl_mem CreateBuffer(cl_GLuint vbo, cl_mem_flags permission);

        const cl_command_queue getQueue() const;

        const std::vector<cl_kernel> &getKernels() const;

    private:
        cl_uint                 _platformCount;
        cl_platform_id          *_platforms;
        cl_device_id            _device;
        cl_uint                 _deviceCount;
        cl_context              _context;
        cl_command_queue        _queue;
        cl_int                  _status;
        std::vector<cl_kernel>  _kernels;

        void PreInit() override;
        void Init() override;
        void PostInit() override;


    };
}


#endif //PARTICLE_SYSTEM_OPENCL_H
