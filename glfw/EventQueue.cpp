//
// Created by Raphael DANTZER on 10/24/16.
//

#include "EventQueue.h"
#include "../proxies/GLFW.h"

// /CALLBACKS
static Proxy::GLFW *getProxy(GLFWwindow *w) {
    return (static_cast<Proxy::GLFW *>(glfwGetWindowUserPointer(w)));
}

static void window_pos_callback(GLFWwindow *window, int x, int y) {
    glfw::EventQueue::Event_t event;

    event.type = glfw::EventQueue::EventType_t::WINDOW_MOVED;
    event.pos.x = x;
    event.pos.y = y;
    getProxy(window)->get_queue().enqueue(event);
}

static void window_size_callback(GLFWwindow *window, int width, int height) {
    glfw::EventQueue::Event_t event;

    event.type = glfw::EventQueue::EventType_t::WINDOW_RESIZED;
    event.size.width = width;
    event.size.height = height;
    getProxy(window)->get_queue().enqueue(event);
}

static void window_close_callback(GLFWwindow *window) {
    glfw::EventQueue::Event_t event;

    event.type = glfw::EventQueue::EventType_t::WINDOW_CLOSED;
    getProxy(window)->get_queue().enqueue(event);
}

static void window_refresh_callback(GLFWwindow *window) {
    glfw::EventQueue::Event_t event;

    event.type = glfw::EventQueue::EventType_t::WINDOW_REFRESH;
    getProxy(window)->get_queue().enqueue(event);
}

static void window_focus_callback(GLFWwindow *window, int focused) {
    glfw::EventQueue::Event_t event;

    if (focused)
        event.type = glfw::EventQueue::EventType_t::WINDOW_FOCUSED;
    else
        event.type = glfw::EventQueue::EventType_t::WINDOW_DEFOCUSED;
    getProxy(window)->get_queue().enqueue(event);
}

static void window_iconify_callback(GLFWwindow *window, int iconified) {
    glfw::EventQueue::Event_t event;

    if (iconified)
        event.type = glfw::EventQueue::EventType_t::WINDOW_ICONIFIED;
    else
        event.type = glfw::EventQueue::EventType_t::WINDOW_RESTORED;
    getProxy(window)->get_queue().enqueue(event);
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glfw::EventQueue::Event_t event;

    event.type = glfw::EventQueue::EventType_t::FRAMEBUFFER_RESIZED;
    event.size.width = width;
    event.size.height = height;
    getProxy(window)->get_queue().enqueue(event);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    glfw::EventQueue::Event_t event;

    if (action == GLFW_PRESS)
        event.type = glfw::EventQueue::EventType_t::KEY_PRESSED;
    else if (action == GLFW_RELEASE)
        event.type = glfw::EventQueue::EventType_t::KEY_RELEASED;
    else
        event.type = glfw::EventQueue::EventType_t::KEY_REPEATED;
    event.key.scancode = scancode;
    event.key.mods = mods;
    event.key.key = key;
    getProxy(window)->get_queue().enqueue(event);
}

static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    glfw::EventQueue::Event_t event;

    if (action == GLFW_PRESS)
        event.type = glfw::EventQueue::EventType_t::BUTTON_PRESSED;
    else
        event.type = glfw::EventQueue::EventType_t::BUTTON_RELEASED;
    event.button.button = button;
    event.button.mods = mods;
    getProxy(window)->get_queue().enqueue(event);
}

static void mouse_scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    glfw::EventQueue::Event_t event;

    event.type = glfw::EventQueue::EventType_t::SCROLL;
    event.pos.x = xoffset;
    event.pos.y = yoffset;
    getProxy(window)->get_queue().enqueue(event);
}

static void mouse_pos_callback(GLFWwindow *window, double xpos, double ypos) {
    glfw::EventQueue::Event_t event;

    event.type = glfw::EventQueue::EventType_t::CURSOR_MOVED;
    event.pos.x = xpos;
    event.pos.y = ypos;
    getProxy(window)->get_queue().enqueue(event);
}

static void mouse_pos_enter_callback(GLFWwindow *window, int entered) {
    glfw::EventQueue::Event_t event;

    if (entered)
        event.type = glfw::EventQueue::EventType_t::CURSOR_ENTERED;
    else
        event.type = glfw::EventQueue::EventType_t::CURSOR_LEFT;
    getProxy(window)->get_queue().enqueue(event);
}

static void file_drop_callback(GLFWwindow *window, int count, const char **paths) {
    glfw::EventQueue::Event_t event;

    event.type = glfw::EventQueue::EventType_t::FILE_DROPPED;
    event.file.count = count;
    event.file.paths.reserve(static_cast<unsigned long>(count));
    for (int i = 0; i < count; ++i)
        event.file.paths.push_back(paths[i]);
    getProxy(window)->get_queue().enqueue(event);
}

static void char_mods_callback(GLFWwindow *window, unsigned int codepoint, int mods) {
    glfw::EventQueue::Event_t event;

    event.type = glfw::EventQueue::EventType_t::CHARACTER_INPUT;
    event.character.codepoint = codepoint;
    event.character.mods = mods;
    getProxy(window)->get_queue().enqueue(event);
}

glfw::EventQueue::EventQueue(GLFWwindow *boundWindow) : _boundWindow(boundWindow) {
    glfwSetWindowPosCallback(_boundWindow, window_pos_callback);
    glfwSetWindowSizeCallback(_boundWindow, window_size_callback);
    glfwSetWindowCloseCallback(_boundWindow, window_close_callback);
    glfwSetWindowRefreshCallback(_boundWindow, window_refresh_callback);
    glfwSetWindowIconifyCallback(_boundWindow, window_iconify_callback);
    glfwSetWindowFocusCallback(_boundWindow, window_focus_callback);
    glfwSetFramebufferSizeCallback(_boundWindow, framebuffer_size_callback);
    glfwSetMouseButtonCallback(_boundWindow, mouse_button_callback);
    glfwSetCursorPosCallback(_boundWindow, mouse_pos_callback);
    glfwSetCursorEnterCallback(_boundWindow, mouse_pos_enter_callback);
    glfwSetScrollCallback(_boundWindow, mouse_scroll_callback);
    glfwSetKeyCallback(_boundWindow, key_callback);
    glfwSetCharModsCallback(_boundWindow, char_mods_callback);
    glfwSetDropCallback(_boundWindow, file_drop_callback);
    _queue.reserve(EVENT_QUEUE_MAXIMUM_SIZE);
}

glfw::EventQueue::~EventQueue() {
}

void glfw::EventQueue::enqueue(Event_t &event) {
    _queue.push_back(event);
}

std::vector<glfw::EventQueue::Event_t> &glfw::EventQueue::get_queue() {
    return _queue;
}

void glfw::EventQueue::dequeue() {
    _queue.clear();
}

glfw::EventQueue::EventQueue() {

}

