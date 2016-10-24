//
// Created by Raphael DANTZER on 10/24/16.
//

#ifndef PARTICLE_SYSTEM_EVENTQUEUE_H
#define PARTICLE_SYSTEM_EVENTQUEUE_H

#include <GLFW/glfw3.h>
#include <string>
#include <vector>
/**
 ** GLFW event pool class idea taken from glfw/gleq
 */

#define EVENT_QUEUE_MAXIMUM_SIZE        (512)

namespace glfw {
    class EventQueue {
    public:
        typedef enum class e_EventType {
            NONE = 0x0,
            WINDOW_MOVED,
            WINDOW_RESIZED,
            WINDOW_CLOSED,
            WINDOW_REFRESH,
            WINDOW_FOCUSED,
            WINDOW_DEFOCUSED,
            WINDOW_ICONIFIED,
            WINDOW_RESTORED,
            FRAMEBUFFER_RESIZED,
            BUTTON_PRESSED,
            BUTTON_RELEASED,
            CURSOR_MOVED,
            CURSOR_ENTERED,
            CURSOR_LEFT,
            SCROLL,
            KEY_PRESSED,
            KEY_REPEATED,
            KEY_RELEASED,
            CHARACTER_INPUT,
            FILE_DROPPED
        } EventType_t;

        typedef struct Event {
            EventType_t     type;
            union {
                struct {
                    double x;
                    double y;
                } pos;
                struct {
                    int width;
                    int height;
                } size;
                struct {
                    int key;
                    int scancode;
                    int mods;
                } key;
                struct {
                    int button;
                    int mods;
                } button;
                struct {
                    unsigned int codepoint;
                    int mods;
                } character;
                struct {
                    std::vector<std::string> paths;
                    int count;
                } file;
            };
        } Event_t;

        EventQueue();
        EventQueue(GLFWwindow *_boundWindow);
        ~EventQueue();

        std::vector<Event_t>    &get_queue();
        void                    dequeue();
        void                    enqueue(Event_t &event);

    private:
        GLFWwindow              *_boundWindow;
        std::vector<Event_t>    _queue;
    };
}


#endif //PARTICLE_SYSTEM_EVENTQUEUE_H
