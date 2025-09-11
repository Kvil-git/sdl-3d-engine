#include "InputHandler.h"
#include "../../Events/InputEvents.h"


void InputHandler::AddEventToProcessingQueue(const SDL_Event &event) {
    eventsToProcess.push_back(event);
}


void InputHandler::Update(){
    for(int i=0; i<SDL_NUM_SCANCODES; i++){
        lastFrameKeyStates[i] = keyStates[i];
    }

    for(int i=0; i<5 + 1; i++){
        lastFrameMouseButtonStates[i] = mouseButtonStates[i];
    }

    for(const auto &event : eventsToProcess){
        switch(event.type){
            case SDL_KEYDOWN:
                keyStates[event.key.keysym.scancode] = true;
                eventController.Emit(
                    KeyboardEvent{event.key.keysym.sym, true}
                );
                break;
            case SDL_KEYUP:
                keyStates[event.key.keysym.scancode] = false;
                eventController.Emit(
                    KeyboardEvent{event.key.keysym.sym, false}
                );
                break;
            case SDL_MOUSEMOTION:
                mouseX = event.motion.x;
                mouseY = event.motion.y;
                mouseRelativeX = event.motion.xrel;
                mouseRelativeY = event.motion.yrel;
                eventController.Emit(
                    MouseMotionEvent{mouseX, mouseY, mouseRelativeX, mouseRelativeY}
                );
                break;
            case SDL_MOUSEBUTTONDOWN:
                {
                    uint8_t button = event.button.button;

                    if (button >= 1 && button <= 5) {
                        mouseButtonStates[button] = true;
                    }
                    eventController.Emit(
                        MouseButtonEvent{button, true, mouseX, mouseY}
                    );
                }
                
                break;
            case SDL_MOUSEBUTTONUP:
                {
                    uint8_t button = event.button.button;

                    if (button >= 1 && button <= 5) {
                        mouseButtonStates[button] = false;
                    }
                    eventController.Emit(
                        MouseButtonEvent{button, false, mouseX, mouseY}
                    );
                }
                break;
        }
    }
    eventsToProcess.clear();
}