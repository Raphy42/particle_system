//
// Created by Raphaël Dantzer on 08/10/16.
//

#ifndef PARTICLE_SYSTEM_UTILS_H
#define PARTICLE_SYSTEM_UTILS_H

#include <OpenCL/opencl.h>
#include <iostream>

namespace OpenCL {
    void getStatus(cl_int error, const char *assertion);
}

#endif //PARTICLE_SYSTEM_UTILS_H
