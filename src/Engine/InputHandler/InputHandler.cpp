#include "InputHandler.h"


void InputHandler::AddEventToProcessingQueue(const SDL_Event &event) {
    eventsToProcess.push_back(event);
}


void InputHandler::Update(){
    for(int i=0; i<SDL_NUM_SCANCODES; i++){
        lastFrameKeyStates[i] = keyStates[i];
    }

    for(int i=0; i<5; i++){
        lastFrameMouseButtonStates[i] = mouseButtonStates[i];
    }

    for(const auto &event : eventsToProcess){
        switch(event.type){
            case SDL_KEYDOWN:
                keyStates[event.key.keysym.scancode] = true;
                break;
            case SDL_KEYUP:
                keyStates[event.key.keysym.scancode] = false;
                break;
            case SDL_MOUSEMOTION:
                mouseX = event.motion.x;
                mouseY = event.motion.y;
                mouseRelativeX = event.motion.xrel;
                mouseRelativeY = event.motion.yrel;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button >= 1 && event.button.button <= 5) {
                    mouseButtonStates[event.button.button - 1] = true;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button >= 1 && event.button.button <= 5) {
                    mouseButtonStates[event.button.button - 1] = false;
                }
                break;
        }
    }
    eventsToProcess.clear();
}