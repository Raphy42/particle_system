#include <iostream>
#include "GLFW.h"
#include "src/opencl/SharedContext.h"
#include "src/opencl/utils.h"

#define MESH_WIDTH 1200
#define MESH_HEIGHT 800

int main() {

    GLFW graphic_context(1200, 800, "Test");
    OpenCL::SharedContext *context = new OpenCL::SharedContext();

    GLuint vbo;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 100, NULL, GL_STATIC_DRAW);
    glVertexPointer(4, GL_FLOAT, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    cl_int err;
    cl_mem cl_buffer = clCreateFromGLBuffer(context->getContext(), CL_MEM_READ_WRITE, vbo, &err);

    err = clEnqueueAcquireGLObjects(context->getQueue(), 1, &cl_buffer, 0, 0, 0);
    OpenCL::getStatus(err, "clEnqueueAcquireGLObjects");
    //EXECUTE KERNEL HERE
    err = clEnqueueReleaseGLObjects(context->getQueue(), 1, &cl_buffer, 0, 0, 0);
    OpenCL::getStatus(err, "clEnqueueReleaseGLObjects");
    err = clFlush(context->getQueue());

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    while (graphic_context.should_exit())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        graphic_context.render();
    }
    return (0);
}