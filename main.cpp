//
// Created by Raphaël Dantzer on 12/10/16.
//

#include "utils/FileLogger.h"
#include "opengl/Program.h"
#include "proxies/OpenCL.h"
#include "opengl/Buffer.h"
#include "opengl/GLFactory.h"
#include <glm/gtc/type_ptr.hpp>

#define PARTICLE_COUNT 1000000
template <typename T>
    constexpr T WIDTH = 2600;

template <typename T>
    constexpr T HEIGHT = 1300;

float mouse_x, mouse_y;
bool stop = true, grab = false;
float attractor = 12.f;

glm::vec4       get3DNDC(glm::mat4 view, glm::mat4 projection)
{
    float x = 2.0f * mouse_x / WIDTH<float> - 1;
    float y = 1.0f - (2.0f * mouse_y) / HEIGHT<float>;

    glm::mat4 viewProjectionInverse = glm::inverse(view * projection);
    glm::vec4 point3D(x, y, 0.f, 1.f);
    glm::vec4 pt = glm::normalize(viewProjectionInverse * point3D);
    pt.w = attractor;
    return pt;
}

int main(void)
{
    FLOG_INFO("main start");
    Proxy::GLFW glfw(std::pair<int, int>(WIDTH<int>, HEIGHT<int>), "Test", std::pair<int, int>(4, 1));
    Proxy::OpenCL cl;
    cl.CreateKernelFromFile("./assets/kernels/particle.cl", "particle_init_sphere");
    cl.CreateKernelFromProgram("particle_init_cube");
    cl.CreateKernelFromProgram("particle_update");
    cl.CreateKernelFromProgram("particle_update_flow");
    OpenGL::Program program("./assets/shaders/particle_vs.glsl", "./assets/shaders/particle_fs.glsl");

    GLFactory factory;

    factory.setUsage(GL_STATIC_DRAW);
    Buffer *buffer = new Buffer();

    glGenVertexArrays(1, &buffer->_vao);
    GLuint vbos[2];
    glGenBuffers(2, vbos);

    glBindVertexArray(buffer->_vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * PARTICLE_COUNT, nullptr, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);


    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * PARTICLE_COUNT, nullptr, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    glFinish();

    float deltaTime = static_cast<float >(glfwGetTime());

    cl_mem pos = cl.CreateBufferFromVBO(vbos[0], CL_MEM_READ_WRITE);
    cl_mem cl_cursor = cl.CreateBuffer(sizeof(float) * 4, CL_MEM_READ_ONLY, nullptr);
    cl_mem vel = cl.CreateBufferFromVBO(vbos[1], CL_MEM_READ_WRITE);
    cl_mem delta = cl.CreateBuffer(sizeof(float), CL_MEM_READ_ONLY, nullptr);

    cl.getStatus(clSetKernelArg(cl.getKernel("particle_init_sphere"), 0, sizeof(cl_mem), (void *)&pos), "clSetKernelArg");

    cl.getStatus(clEnqueueAcquireGLObjects(cl.getQueue(), 1, &pos, 0, nullptr, nullptr), "clEnqueueAcquireGLObjects");
    size_t global_item_size = PARTICLE_COUNT;
    cl.getStatus(clEnqueueNDRangeKernel(cl.getQueue(), cl.getKernel("particle_init_sphere"), 1, nullptr,
                                        &global_item_size, nullptr, 0, nullptr, nullptr), "clEnqueueNDRangeKernel");
    cl.getStatus(clEnqueueReleaseGLObjects(cl.getQueue(), 1, &pos, 0, nullptr, nullptr), "clEnqueueReleaseGLObjects");

    clFinish(cl.getQueue());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);

    GLint mvp_id = program.uniform("mvp");
    GLint center_id = program.uniform("center");

    glm::mat4 model;
    glm::mat4 perspective = glm::perspective(68.f, WIDTH<float> / HEIGHT<float>, 0.1f, 1000.f);
    FPSCamera camera(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, -1.f, 0.f), -90.f, 0.f);

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
                case GLFW_KEY_SPACE:
                    stop = action == GLFW_PRESS == !stop;
                    break;
                case GLFW_KEY_LEFT_SHIFT:
                case GLFW_KEY_RIGHT_SHIFT:
                    grab = action == GLFW_PRESS == !grab;
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
        GLfloat xoffset = static_cast<GLfloat>(xpos - lastX), yoffset = static_cast<GLfloat>(ypos - lastY);
        lastX = xpos;
        lastY = ypos;
        //DIRTY
        mouse_x = static_cast<float>(xpos);
        mouse_y = static_cast<float>(ypos);
        if (grab)
        {
            cam.mouseEvent(xoffset, yoffset);
            ctx->setCamera(cam);
        }
    });

    glfw.bindScrollCallback([](GLFWwindow *window, double xoffset, double yoffset){
        Proxy::GLFW *ctx = static_cast<Proxy::GLFW *>(glfwGetWindowUserPointer(window));
        FPSCamera cam = ctx->getCamera();

        cam.mouseScrollEvent(static_cast<GLfloat>(yoffset));
        ctx->setCamera(cam);
    });
    glfw.bindMouseCallback([](GLFWwindow *window, int button, int action, int mods){
        if (action == GLFW_PRESS)
            attractor = 50.f;
        else
            attractor = 12.f;
    });

    cl_kernel update = cl.getKernel("particle_update_flow");

    while (!glfwWindowShouldClose(glfw.getWindow()))
    {
        deltaTime = static_cast<float>(glfwGetTime());
        glm::mat4 view = glfw.getCamera().getViewMat4();
        glm::mat4 mvp = perspective * view * model;
        glm::vec4 cursor = get3DNDC(view, perspective);
        if (!stop)
        {
            glFinish();

            cl.getStatus(clEnqueueWriteBuffer(cl.getQueue(), delta, CL_TRUE, 0, sizeof(float), &deltaTime, 0, nullptr,
                                              nullptr), "clEnqueueWriteBuffer - deltaTime");

            cl.getStatus(clEnqueueWriteBuffer(cl.getQueue(), cl_cursor, CL_TRUE, 0, sizeof(float) * 4, glm::value_ptr(cursor),
                                              0, nullptr, nullptr), "clEnqueueWriteBuffer - cl_cursor");

            cl.getStatus(clSetKernelArg(update, 0, sizeof(cl_mem), (void *)&pos), "clSetKernelArg - pos");
            cl.getStatus(clSetKernelArg(update, 1, sizeof(cl_mem), (void *)&vel), "clSetKernelArg - velocity");
            cl.getStatus(clSetKernelArg(update, 2, sizeof(cl_mem), (void *)&cl_cursor), "clSetKernelArg - cursor");
            cl.getStatus(clSetKernelArg(update, 3, sizeof(cl_mem), (void *)&delta), "clSetKernelArg - delta");

            cl.getStatus(clEnqueueAcquireGLObjects(cl.getQueue(), 1, &pos, 0, nullptr, nullptr), "clEnqueueAcquireGLObjects");
            cl.getStatus(clEnqueueAcquireGLObjects(cl.getQueue(), 1, &vel, 0, nullptr, nullptr), "clEnqueueAcquireGLObjects");

            cl.getStatus(clEnqueueNDRangeKernel(cl.getQueue(), update, 1, nullptr,
                                                &global_item_size, nullptr, 0, nullptr, nullptr), "clEnqueueNDRangeKernel");

            cl.getStatus(clEnqueueReleaseGLObjects(cl.getQueue(), 1, &pos, 0, nullptr, nullptr), "clEnqueueReleaseGLObjects");
            cl.getStatus(clEnqueueReleaseGLObjects(cl.getQueue(), 1, &vel, 0, nullptr, nullptr), "clEnqueueReleaseGLObjects");

            clFinish(cl.getQueue());
        }

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program.bind();
        glUniformMatrix4fv(mvp_id, 1, GL_FALSE, glm::value_ptr(mvp));
        glUniform4fv(center_id, 1, glm::value_ptr(cursor));
        glBindVertexArray(buffer->_vao);
        glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);
        glBindVertexArray(0);
        glfwSwapBuffers(glfw.getWindow());
        glfwPollEvents();
        glUseProgram(0);
    }
    return (0);
}