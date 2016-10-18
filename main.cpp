//
// Created by Raphaël Dantzer on 12/10/16.
//

#include "utils/FileLogger.h"
#include "opengl/Program.h"
#include "proxies/OpenCL.h"
#include <OpenGL/gl3.h>
#include <glm/glm.hpp>

#define PARTICLE_COUNT 1000

int main(void)
{
    FLOG_INFO("main start");
    Proxy::GLFW glfw(std::pair<int, int>(1200, 800), "Test", std::pair<int, int>(4, 1));
    Proxy::OpenCL cl;
    cl.CreateKernel("./assets/kernels/particle.cl", "particle_init");
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

//    glm::mat4 perspective = glm::perspective(90.f, 1200 / 800, 0.01f, 1000.f);

    cl_mem pos = cl.CreateBuffer(vbo, CL_MEM_READ_WRITE);
    clEnqueueWriteBuffer(cl.getQueue(), pos, CL_TRUE, 0, PARTICLE_COUNT * 4 * sizeof(float), nullptr, 0, nullptr, nullptr);
    clSetKernelArg(cl.getKernels()[0], 0, sizeof(cl_mem), (void *)&pos);

    size_t global_item_size = PARTICLE_COUNT, local_item_size = 1;

    clEnqueueNDRangeKernel(cl.getQueue(), cl.getKernels()[0], 1, nullptr, &global_item_size, &local_item_size, 0,
                           nullptr, nullptr);

    clFlush(cl.getQueue());
    clFlush(cl.getQueue());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);

    while (!glfwWindowShouldClose(glfw.getWindow()))
    {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program.bind();
        glBindVertexArray(vao);
        glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);
        glBindVertexArray(0);
        glFinish();
        glfwSwapBuffers(glfw.getWindow());
        glfwPollEvents();
    }
    return (0);
}