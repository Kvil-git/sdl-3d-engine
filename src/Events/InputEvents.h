#ifndef INPUT_EVENTS_H
#define INPUT_EVENTS_H

#include <SDL2/SDL.h>

struct KeyboardEvent {
    SDL_Keycode key;
    bool pressed;
};

struct MouseButtonEvent {
    uint8_t button;
    bool pressed;
    int mouseX;
    int mouseY;
};

struct MouseMotionEvent {
    int mouseX;
    int mouseY;
    int relX;
    int relY;
};

#endif