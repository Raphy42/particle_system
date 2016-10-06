#include <iostream>
#include <GLFW/glfw3.h>
#include "GLFW.h"

int main() {
    GLFW graphic_context(1200, 800, "Test");

    graphic_context.render();
    return (0);
}