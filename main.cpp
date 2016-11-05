//
// Created by Raphaël Dantzer on 12/10/16.
//

#include "utils/FileLogger.h"
#include "opengl/Program.h"
#include "proxies/OpenCL.h"
#include "opengl/Buffer.h"
#include "opengl/GLFactory.h"
#include "utils/FPSCamera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define PARTICLE_COUNT 3000000

int main(void)
{
    FLOG_INFO("main start");
    Proxy::GLFW glfw(std::pair<int, int>(2000, 1600), "Test", std::pair<int, int>(4, 1));
    Proxy::OpenCL cl;
    cl.CreateKernelFromFile("./assets/kernels/particle.cl", "particle_init_sphere");
    cl.CreateKernelFromProgram("particle");
    cl.CreateKernelFromProgram("particle_init_cube");
    OpenGL::Program program("./assets/shaders/particle_vs.glsl", "./assets/shaders/particle_fs.glsl");

    GLFactory factory;

    factory.setUsage(GL_STREAM_DRAW);
    std::unique_ptr<Buffer> buffer(factory.RegisterF(nullptr, sizeof(float) * 4 * PARTICLE_COUNT, 4, GL_ARRAY_BUFFER));
    glFinish();


    //CL OPS
    cl_float4 *cursor_pos = new cl_float4;

    cl_mem pos = cl.CreateBufferFromVBO(buffer->_vbo, CL_MEM_READ_WRITE);
    cl_mem cl_cursor = cl.CreateBuffer(sizeof(cl_cursor), CL_MEM_COPY_HOST_PTR, cursor_pos);

    cl.getStatus(clSetKernelArg(cl.getKernels()[0], 0, sizeof(cl_mem), (void *)&pos), "clSetKernelArg");

    cl.getStatus(clEnqueueAcquireGLObjects(cl.getQueue(), 1, &pos, 0, nullptr, nullptr), "clEnqueueAcquireGLObjects");
    size_t global_item_size = PARTICLE_COUNT, local_item_size = 1;
    cl.getStatus(clEnqueueNDRangeKernel(cl.getQueue(), cl.getKernels()[0], 1, nullptr,
                                        &global_item_size, &local_item_size, 0, nullptr, nullptr), "clEnqueueNDRangeKernel");
    cl.getStatus(clEnqueueReleaseGLObjects(cl.getQueue(), 1, &pos, 0, nullptr, nullptr), "clEnqueueReleaseGLObjects");

    clFinish(cl.getQueue());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);

    GLint mvp_id = program.uniform("mvp");

    glm::mat4 model;
    glm::mat4 perspective = glm::perspective(68.f, 1200.f / 800.f, 0.1f, 1000.f);
    FPSCamera camera(glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.f, 1.f, 0.f), -90.f, 0.f);

    glfw.bindKeyCallback([](GLFWwindow *window, int key, int scancode, int action, int mode){
        Proxy::GLFW *ctx = static_cast<Proxy::GLFW *>(glfwGetWindowUserPointer(window));
        FPSCamera cam = ctx->getCamera();
            switch (key) {
                case GLFW_KEY_ESCAPE:
                    glfwSetWindowShouldClose(window, true);
                    break;
                case GLFW_KEY_W:
                    cam.keyboardEvent(FPSCamera::e_CameraMovement::FORWARD, .1f);
                    break;
                case GLFW_KEY_S:
                    cam.keyboardEvent(FPSCamera::e_CameraMovement::BACKWARD, .1f);
                    break;
                case GLFW_KEY_D:
                    cam.keyboardEvent(FPSCamera::e_CameraMovement::LEFT, .1f);
                    break;
                case GLFW_KEY_A:
                    cam.keyboardEvent(FPSCamera::e_CameraMovement::RIGHT, .1f);
                    break;
                default:
                    break;
            }
        ctx->setCamera(cam);
    });

    glfw.bindCursorPosCallback([](GLFWwindow *window, double xpos, double ypos){
        Proxy::GLFW *ctx = static_cast<Proxy::GLFW *>(glfwGetWindowUserPointer(window));
        FPSCamera cam = ctx->getCamera();
        static bool first = true;
        static double lastX, lastY;

        if (first)
        {
            lastX = xpos;
            lastY = ypos;
            first = false;
        }
        GLfloat xoffset = xpos - lastX, yoffset = ypos - lastY;
        lastX = xpos;
        lastY = ypos;
        cam.mouseEvent(xoffset, yoffset);
        ctx->setCamera(cam);
    });

    glfw.bindScrollCallback([](GLFWwindow *window, double xoffset, double yoffset){
        Proxy::GLFW *ctx = static_cast<Proxy::GLFW *>(glfwGetWindowUserPointer(window));
        FPSCamera cam = ctx->getCamera();

        cam.mouseScrollEvent(static_cast<GLfloat>(yoffset));
        ctx->setCamera(cam);
    });

    while (!glfwWindowShouldClose(glfw.getWindow()))
    {
        glm::mat4 view = glfw.getCamera().getViewMat4();
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
        glBindVertexArray(buffer->_vao);
        glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);
        glBindVertexArray(0);
        glfwSwapBuffers(glfw.getWindow());
        glfwPollEvents();
    }
    return (0);
}