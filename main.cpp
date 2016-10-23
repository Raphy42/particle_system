//
// Created by Raphaël Dantzer on 12/10/16.
//

#include "utils/FileLogger.h"
#include "opengl/Program.h"
#include "proxies/OpenCL.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <OpenCL/opencl.h>

#define PARTICLE_COUNT 1000000

int main(void)
{
    FLOG_INFO("main start");
    Proxy::GLFW glfw(std::pair<int, int>(2000, 1600), "Test", std::pair<int, int>(4, 1));
    Proxy::OpenCL cl;
    cl.CreateKernelFromFile("./assets/kernels/particle.cl", "particle_init");
    cl.CreateKernelFromProgram("particle");
    OpenGL::Program program("./assets/shaders/particle_vs.glsl", "./assets/shaders/particle_fs.glsl");
    GLuint vao, vbo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * PARTICLE_COUNT, nullptr, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glFinish();


    //CL OPS
    cl_float4 *cursor_pos = new cl_float4;

    cl_mem pos = cl.CreateBufferFromVBO(vbo, CL_MEM_READ_WRITE);
    cl_mem cl_cursor = cl.CreateBuffer(sizeof(cl_cursor), CL_MEM_COPY_HOST_PTR, cursor_pos);

    cl.getStatus(clSetKernelArg(cl.getKernels()[0], 0, sizeof(cl_mem), (void *)&pos), "clSetKernelArg");

    cl.getStatus(clEnqueueAcquireGLObjects(cl.getQueue(), 1, &pos, 0, nullptr, nullptr), "clEnqueueAcquireGLObjects");
    size_t global_item_size = PARTICLE_COUNT, local_item_size = 64;
    cl.getStatus(clEnqueueNDRangeKernel(cl.getQueue(), cl.getKernels()[0], 1, nullptr,
                                        &global_item_size, &local_item_size, 0, nullptr, nullptr), "clEnqueueNDRangeKernel");
    cl.getStatus(clEnqueueReleaseGLObjects(cl.getQueue(), 1, &pos, 0, nullptr, nullptr), "clEnqueueReleaseGLObjects");

    clFinish(cl.getQueue());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);

    GLint mvp_id = program.uniform("mvp");

    while (!glfwWindowShouldClose(glfw.getWindow()))
    {
        glm::mat4 model;
        glm::mat4 view = glm::lookAt(glm::vec3(5 * sin(glfwGetTime()), 0, 5 * cos(glfwGetTime())),
                                     glm::vec3(0.0f, 0.0f, 0.0f),
                                     glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 perspective = glm::perspective(68.f, 1200.f / 800.f, 0.1f, 1000.f);
        glm::mat4 mvp = perspective * view * model;

//        glfwGetCursorPos(glfw.getWindow(), (double *) &cursor_pos->s[0], (double *) &cursor_pos->s[1]);
//        cursor_pos->s[2] = 1.0f;
//        cursor_pos->s[3] = 1.0f;
//
//        glFinish();
//
//        cl.getStatus(clSetKernelArg(cl.getKernels()[1], 0, sizeof(cl_mem), (void *)&pos), "clSetKernelArg");
//        cl.getStatus(clSetKernelArg(cl.getKernels()[1], 1, sizeof(cl_mem), (void *)&cl_cursor), "clSetKernelArg");
//        cl.getStatus(clEnqueueAcquireGLObjects(cl.getQueue(), 1, &pos, 0, nullptr, nullptr), "clEnqueueAcquireGLObjects");
//        cl.getStatus(clEnqueueNDRangeKernel(cl.getQueue(), cl.getKernels()[1], 1, nullptr,
//                                            &global_item_size, &local_item_size, 0, nullptr, nullptr), "clEnqueueNDRangeKernel");
//        cl.getStatus(clEnqueueReleaseGLObjects(cl.getQueue(), 1, &pos, 0, nullptr, nullptr), "clEnqueueReleaseGLObjects");
//
//        clFinish(cl.getQueue());

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program.bind();
        glUniformMatrix4fv(mvp_id, 1, GL_FALSE, glm::value_ptr(mvp));
        glBindVertexArray(vao);
        glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);
        glBindVertexArray(0);
        glfwSwapBuffers(glfw.getWindow());
        glfwPollEvents();
    }
    return (0);
}