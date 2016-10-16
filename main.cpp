//
// Created by Raphaël Dantzer on 12/10/16.
//

#include "utils/FileLogger.h"
#include "proxies/GLFW.h"

int main(void)
{
    FLOG_INFO("main start");
    Proxy::GLFW glfw(std::pair<int, int>(1200, 800), "Test", std::pair<int, int>(4, 1));

    while (!glfwWindowShouldClose(glfw.getWindow()))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwSwapBuffers(glfw.getWindow());
    }
    return (0);
}